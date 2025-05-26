// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdocklayout_p.h"
#include "qquickdocklayout_p_p.h"
#include "qquickdockgroup_p.h"
#include "qquickcontentitem_p.h"
#include "qquickdockcontainer_p_p.h"
#include "qquickdockpanel_p_p.h"

#include <cstdlib>

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockLayout
    \inherits SplitView
    \instantiates QQuickDockLayout
    \inqmlmodule QtQuick.Controls
    \since 6.12
    \brief Internal layout used to arrange dock widget groups in split views.

 DockLayout is an internal type that behaves like a \l SplitView and is
 responsible for arranging docked groups of widgets inside a DockContainer.

 Each DockLayout can contain multiple \l DockPanelGroup items, each of which
 may host a tabbed set of dockable panels.

 This class provides the core layouting engine behind the docking system and is
 typically not instantiated directly in QML.

 \note This type is used internally by \l DockContainer and should not be
 manipulated directly unless customizing the docking system implementation.

 \sa DockContainer, DockPanelGroup, SplitView
*/

// Q_STATIC_LOGGING_CATEGORY(qlcQQuickDockLayout, "qt.quick.controls.docklayout")
// Q_STATIC_LOGGING_CATEGORY(qlcQQuickDockLayoutPointer, "qt.quick.controls.docklayout.pointer")
// Q_STATIC_LOGGING_CATEGORY(qlcQQuickDockLayoutState, "qt.quick.controls.docklayout.state")

namespace {

static constexpr qreal kMinSize = 20.0;

QVector<qreal> calculateProportionalSizes(const QVector<qreal> &sizes, qreal availableSpace)
{
    const int n = sizes.size();
    if (n == 0)
        return {};

    QVector<qreal> result(n);

    qreal total = 0.0;
    for (qreal s : sizes)
        total += qMax(s, kMinSize);

    if (total <= 0.0 || availableSpace <= 0.0) {
        const qreal each = qMax(availableSpace / n, kMinSize);
        for (int i = 0; i < n; ++i)
            result[i] = each;
        return result;
    }

    qreal used = 0.0;
    for (int i = 0; i < n; ++i) {
        const qreal ratio = qMax(sizes[i], kMinSize) / total;
        result[i] = qMax(availableSpace * ratio, kMinSize);
        used += result[i];
    }

    if (n > 0) {
        result.last() += (availableSpace - used);
        result.last() = qMax(result.last(), kMinSize);
    }

    return result;
}

QVector<qreal> calculateSizesWithMainItem(const QVector<qreal> &sizes, qreal availableSpace,
                                          int mainIndex)
{
    const int n = sizes.size();
    if (n == 0)
        return {};

    if (mainIndex < 0 || mainIndex >= n)
        return calculateProportionalSizes(sizes, availableSpace);

    QVector<qreal> result(n, kMinSize);

    qreal nonMainDesired = 0.0;
    for (int i = 0; i < n; ++i) {
        if (i == mainIndex)
            continue;
        nonMainDesired += qMax(sizes[i], kMinSize);
    }

    const qreal remainingForMain = availableSpace - nonMainDesired;

    if (remainingForMain >= kMinSize) {
        for (int i = 0; i < n; ++i) {
            if (i == mainIndex) {
                result[i] = remainingForMain;
            } else {
                result[i] = qMax(sizes[i], kMinSize);
            }
        }
    } else {

        result[mainIndex] = kMinSize;
        const qreal spaceForNonMains = availableSpace - kMinSize;

        if (spaceForNonMains <= 0.0) {
            for (int i = 0; i < n; ++i)
                result[i] = kMinSize;
        } else if (nonMainDesired <= 0.0) {
            result[mainIndex] = qMax(availableSpace, kMinSize);
        } else {
            for (int i = 0; i < n; ++i) {
                if (i == mainIndex)
                    continue;

                const qreal desired = qMax(sizes[i], kMinSize);
                const qreal ratio = desired / nonMainDesired;
                result[i] = qMax(spaceForNonMains * ratio, kMinSize);
            }
        }
    }

    qreal used = 0.0;
    for (qreal s : result)
        used += s;

    if (!qFuzzyCompare(used, availableSpace)) {
        result[mainIndex] += (availableSpace - used);
        result[mainIndex] = qMax(result[mainIndex], kMinSize);
    }

    return result;
}
} // namespace

void QQuickDockLayoutPrivate::updateHoveredHandle(QQuickItem *hoveredItem)
{
    const int oldHoveredHandleIndex = hoveredHandleIndex;
    hoveredHandleIndex = handleItems.indexOf(hoveredItem);
    if (hoveredHandleIndex == oldHoveredHandleIndex)
        return;

    // First, clear the hovered flag of any previously-hovered handle.
    if (oldHoveredHandleIndex != -1) {
        auto oldHoveredHandle = handleItems.at(oldHoveredHandleIndex);
        auto oldHoveredHandleAttached = qobject_cast<QQuickDockLayoutHandleAttached *>(
                qmlAttachedPropertiesObject<QQuickDockLayoutHandleAttached>(oldHoveredHandle,
                                                                            true));
        QQuickDockLayoutHandleAttachedPrivate::get(oldHoveredHandleAttached)->setHovered(false);
    }

    if (hoveredHandleIndex != -1) {
        auto handleAttached = qobject_cast<QQuickDockLayoutHandleAttached *>(
                qmlAttachedPropertiesObject<QQuickDockLayoutHandleAttached>(hoveredItem, true));
        QQuickDockLayoutHandleAttachedPrivate::get(handleAttached)->setHovered(true);
    } else {
        // qCDebug(qlcQQuickDockLayoutPointer)
        // << "either there is no hovered item or" << hoveredItem << "is not a handle";
    }
}
QQuickDockGroup *QQuickDockLayoutPrivate::createLayout(int index, InsertChildType insertType)
{
    Q_Q(QQuickDockLayout);
    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    auto newLayout = containerPrivate->createSplitView(nullptr);
    newLayout->setOrientation(orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);

    auto sizeData = sizeHash[itemAt(index)];

    auto currentGroup = q->takeItem(index);
    auto newGroup = containerPrivate->createGroup(nullptr);

    QObjectPrivate::connect(newGroup, &QQuickItem::visibleChanged, this,
                            &QQuickDockLayoutPrivate::layout);
    switch (insertType) {
    case AfterCurrent:
        newLayout->addItem(currentGroup);
        newLayout->addItem(newGroup);
        break;
    case BeforeCurrent:
        if (stretchIndex == index)
            stretchIndex++;
        newLayout->addItem(newGroup);
        newLayout->addItem(currentGroup);
        break;
    }
    q->insertItem(index, newLayout);
    sizeHash[newLayout] = sizeData;
    sizeHash.remove(currentGroup);

    ensureThereIsEnoughHandles();
    layout();
    return newGroup;
}

QQuickDockGroup *QQuickDockLayoutPrivate::splitGroup(int index, InsertChildType insertType)
{
    Q_Q(QQuickDockLayout);
    auto containerPrivate = QQuickDockContainerPrivate::get(container);
    auto newGroup = containerPrivate->createGroup(nullptr);
    auto currentItem = q->itemAt(index);

    QObjectPrivate::connect(newGroup, &QQuickItem::visibleChanged, this,
                            &QQuickDockLayoutPrivate::layout);

    switch (insertType) {
    case AfterCurrent:
        if (index <= contentModel->count())
            q->insertItem(index + 1, newGroup);
        else
            q->addItem(newGroup);
        break;
    case BeforeCurrent:
        q->insertItem(index, newGroup);

        if (stretchIndex == index)
            stretchIndex++;
        break;
    }

    auto currentItemSizeData = sizeHash[currentItem];
    ItemSizeData newItemSizeData;

    if (stretchIndex == index) {
        newItemSizeData.currentSize = 200;
        currentItemSizeData.currentSize -= 200;
    } else {
        newItemSizeData.currentSize = currentItemSizeData.currentSize =
                currentItemSizeData.currentSize / 2;
    }

    sizeHash[newGroup] = newItemSizeData;

    ensureThereIsEnoughHandles();
    layout();

    return newGroup;
}

void QQuickDockLayoutPrivate::addDockPanel(QQuickItem *item, QQuickDockPanel *dockPanel,
                                           Qt::Edge edge)
{
    const auto index = contentModel->indexOf(item, nullptr);

    if (index == -1)
        return;

    QQuickDockGroup *group{ nullptr };

    switch (edge) {
    case Qt::TopEdge:
        if (orientation == Qt::Horizontal)
            group = createLayout(index, BeforeCurrent);
        else
            group = splitGroup(index, BeforeCurrent);
        break;
    case Qt::BottomEdge:
        if (orientation == Qt::Horizontal)
            group = createLayout(index, AfterCurrent);
        else
            group = splitGroup(index, AfterCurrent);
        break;
    case Qt::LeftEdge:
        if (orientation == Qt::Horizontal)
            group = splitGroup(index, BeforeCurrent);
        else
            group = createLayout(index, BeforeCurrent);
        break;
    case Qt::RightEdge:
        if (orientation == Qt::Horizontal)
            group = splitGroup(index, AfterCurrent);
        else
            group = createLayout(index, AfterCurrent);
        break;
    }

    if (group)
        group->addItem(dockPanel);
}

bool QQuickDockLayoutPrivate::isMainItem(QQuickItem *item)
{
    if (qobject_cast<QQuickDockGroup *>(item))
        return false;
    if (const auto layout = qobject_cast<QQuickDockLayout *>(item); layout)
        return QQuickDockLayoutPrivate::get(layout)->stretchIndex != -1;
    return true;
}

QQuickDockLayoutPrivate::DropAreaData::DropAreaData(QQuickItem *item, const QRectF &rect,
                                                    QQuickDockLayout *layout, AreaType areaType)
    : item{ item }, rect{ rect }, layout{ layout }, areaType{ areaType }, group{ nullptr }
{
}
QQuickDockLayoutPrivate::DropAreaData::DropAreaData(const QRectF &rect, QQuickDockGroup *group)
    : rect{ rect }, group{ group }
{
}

void QQuickDockLayoutPrivate::iterateForDropArea(QQuickDockContainer *container,
                                                 QList<DropAreaData> &list)
{
    Q_Q(QQuickDockLayout);

    for (auto i = 0; i < contentModel->count(); ++i) {
        auto item = itemAt(i);
        if (const auto group = qobject_cast<QQuickDockGroup *>(item); group)
            list.emplace_back(item,
                              QRectF{ item->mapToItem(container, QPointF{ 0, 0 }), item->size() },
                              q, DropAreaData::AreaType::Group);
        else if (const auto layout = qobject_cast<QQuickDockLayout *>(item); layout) {
            get(layout)->iterateForDropArea(container, list);
        } else
            list.emplace_back(item,
                              QRectF{ item->mapToItem(container, QPointF{ 0, 0 }), item->size() },
                              q, DropAreaData::AreaType::MainItem);
    }
}

void QQuickDockLayoutPrivate::itemVisibilityChanged(QQuickItem *item)
{
    Q_UNUSED(item)
    layout();
}

void QQuickDockLayoutPrivate::layout()
{
    Q_Q(QQuickDockLayout);
    QVector<qreal> sizes;
    auto count = contentModel->count();
    visibleItems.clear();
    for (auto &handle : handleItems)
        handle->setVisible(false);
    for (auto i = 0; i < count; ++i) {
        auto item = itemAt(i);
        if (!item->isVisible()) {
            continue;
        }
        visibleItems << item;
        auto size = sizeHash[item];
        sizes << size.currentSize;
    }

    qreal totalFreeSpace =
            orientation == Qt::Horizontal ? q->availableWidth() : q->availableHeight();
    QVector<qreal> itemSizes;
    if (stretchIndex == -1)
        itemSizes = calculateProportionalSizes(sizes, totalFreeSpace);
    else
        itemSizes = calculateSizesWithMainItem(sizes, totalFreeSpace, stretchIndex);

    qreal usedSpace{ orientation == Qt::Horizontal ? q->leftPadding() : q->topPadding() };

    for (int i = 0; i < visibleItems.size() - 1; ++i) {
        auto item = visibleItems[i];
        QQuickItem *handle{ nullptr };
        if (!item->isVisible())
            continue;

        if (i < visibleItems.size() - 1) {
            handle = handleItems[i];
            handle->setVisible(true);
        }

        item->setVisible(true);
        setItemGeometry(item, usedSpace, itemSizes[i]);
        usedSpace += itemSize(item) + 1;
        if (handle) {
            setItemGeometry(handle, usedSpace);
            usedSpace += itemSize(handle) + 1;
        }
    }

    if (!visibleItems.isEmpty()) {
        auto item = visibleItems.last();
        setItemGeometry(item, usedSpace, totalFreeSpace - usedSpace);
    }
}

void QQuickDockLayoutPrivate::updateSizeDataFromState()
{
    auto usableArea =
            (orientation == Qt::Horizontal ? getContentItem()->width() : getContentItem()->height())
            - handleItemsSize;

    qreal itemSizeSum{};

    for (int i = 0; i < contentModel->count(); ++i) {
        auto item = itemAt(i);
        itemSizeSum += itemSize(item);
    }

    for (int i = 0; i < contentModel->count(); ++i) {
        auto item = itemAt(i);

        auto &sizeData = sizeHash[item];
        sizeData.percentageSize = itemSize(item) / itemSizeSum;
        sizeData.currentSize = sizeData.percentageSize * usableArea;
    }
}

void QQuickDockLayoutPrivate::setItemGeometry(QQuickItem *item, const qreal pos,
                                              const qreal size) const
{
    Q_Q(const QQuickDockLayout);
    if (orientation == Qt::Horizontal) {
        item->setX(pos);
        item->setY(0);
        if (size)
            item->setWidth(size);
        item->setHeight(q->height());
    } else {
        item->setX(0);
        item->setY(pos);
        item->setWidth(q->width());
        if (size)
            item->setHeight(size);
    }
}

qreal QQuickDockLayoutPrivate::itemSize(const QQuickItem *item) const
{
    return orientation == Qt::Horizontal ? item->width() : item->height();
}

void QQuickDockLayoutPrivate::ensureThereIsEnoughHandles()
{
    while (handleItems.size() < contentModel->count() - 1)
        createHandle();

    while (!handleItems.empty() && handleItems.size() >= contentModel->count()) {
        auto h = handleItems.takeAt(0);
        h->setParentItem(nullptr);
        h->deleteLater();
    }

    handleItemsSize = 0;
    for (const auto &handle : handleItems)
        handleItemsSize += orientation == Qt::Horizontal ? handle->width() : handle->height();
}

QQuickItem *QQuickDockLayoutPrivate::createHandle()
{
    Q_Q(QQuickDockLayout);
    QQmlContext *context = handleComponent->creationContext();
    if (!context)
        context = qmlContext(q);

    QQuickItem *handleItem = qobject_cast<QQuickItem *>(handleComponent->beginCreate(context));
    if (handleItem) {
        handleItem->setParent(q);

        handleItems << handleItem;
        handleItem->setParentItem(q);
        handleItem->setAcceptedMouseButtons(Qt::LeftButton);
        handleItem->setKeepMouseGrab(true);
#if QT_CONFIG(cursor)
        handleItem->setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
#endif
        handleComponent->completeCreate();
    }

    return handleItem;
}

bool QQuickDockLayoutPrivate::containsMainItem()
{
    if (stretchIndex != -1)
        return true;

    for (auto i = 0; i < contentModel->count(); ++i) {
        auto item = itemAt(i);
        if (auto child = qobject_cast<QQuickDockLayout *>(item)) {
            if (get(child)->containsMainItem())
                return true;
        }
    }
    return false;
}

void QQuickDockLayoutPrivate::itemAdded(int index, QQuickItem *item)
{
    Q_Q(QQuickDockLayout);

    if (isMainItem(item)) {

        stretchIndex = index;
    }

    ensureThereIsEnoughHandles();

    item->setVisible(true);

    if (!sizeHash.contains(item)) {
        ItemSizeData data;
        auto attached = qobject_cast<QQuickDockLayoutAttached *>(
                qmlAttachedPropertiesObject<QQuickDockLayout>(item, false));

        if (attached) {
            auto attachedPrivate = QQuickDockLayoutAttachedPrivate::get(attached);

            data.currentSize = attachedPrivate->preferredSize;
        } else {
            qreal implicitSize =
                    orientation == Qt::Horizontal ? item->implicitWidth() : item->implicitHeight();
            data.currentSize = qMax(implicitSize, 200.);
        }
        sizeHash.insert(item, data);
    }

    if (q->isComponentComplete())
        layout();
    // QObjectPrivate::connect(item, &QQuickItem::visibleChanged, this,
    // &QQuickDockLayoutPrivate::layoutVisibleItems);
}

void QQuickDockLayoutPrivate::itemRemoved(int index, QQuickItem *item)
{
    Q_UNUSED(index);
    Q_Q(QQuickDockLayout);
    ensureThereIsEnoughHandles();
    sizeHash.remove(item);
    // visibleItems.removeOne(item);
    if (stretchIndex == index)
        stretchIndex = -1;
    else if (stretchIndex > index)
        stretchIndex--;
    layout();
}

void QQuickDockLayoutPrivate::updatePolish()
{
    layout();
}

QQuickItem *QQuickDockLayoutPrivate::getContentItem()
{
    Q_Q(QQuickDockLayout);
    if (QQuickItem *item = QQuickContainerPrivate::getContentItem())
        return item;

    return new QQuickContentItem(q);
}

bool QQuickDockLayoutPrivate::handlePress(const QPointF &point, ulong timestamp)
{
    Q_Q(QQuickDockLayout);

    QQuickContainerPrivate::handlePress(point, timestamp);

    QQuickItem *handle = q->childAt(point.x(), point.y());

    pressedHandleIndex = handleItems.indexOf(handle);

    if (pressedHandleIndex == -1) {
        return true;
    }

    handleMouseClickPos = point;
    resizeHandleItem1 = itemAt(pressedHandleIndex);
    resizeHandleItem2 = itemAt(pressedHandleIndex + 1);

    sizeDataItem1 = &sizeHash[resizeHandleItem1];
    sizeDataItem2 = &sizeHash[resizeHandleItem2];

    if (orientation == Qt::Horizontal) {
        resizeHandleItem1Size = resizeHandleItem1->width();
        resizeHandleItem2Size = resizeHandleItem2->width();
        handlePressPos = handle->x();
    } else {
        resizeHandleItem1Size = resizeHandleItem1->height();
        resizeHandleItem2Size = resizeHandleItem2->height();
        handlePressPos = handle->y();
    }

    resizeArea.setTopLeft(resizeHandleItem1->position());
    resizeArea.setSize(
            { resizeHandleItem1->width() + handle->width() + resizeHandleItem2->width(),
              resizeHandleItem1->height() + handle->height() + resizeHandleItem2->height() });
    return true;
}

bool QQuickDockLayoutPrivate::handleMove(const QPointF &point, ulong timestamp)
{
    Q_UNUSED(timestamp);

    QQuickItem *handle = handleItems[pressedHandleIndex];

    if (orientation == Qt::Horizontal) {
        auto handleX = handlePressPos + point.x() - handleMouseClickPos.x();
        auto size1 = handleX - resizeArea.left();
        auto size2 = resizeArea.width() - size1 - handle->width();

        if (size1 > 0 && size2 > 0) {
            handle->setX(handleX);

            resizeHandleItem1->setWidth(size1);
            resizeHandleItem2->setX(handle->x() + handle->width());
            resizeHandleItem2->setWidth(size2);
        }
    } else {
        auto handleY = handlePressPos + point.y() - handleMouseClickPos.y();
        auto size1 = handleY - resizeArea.top();
        auto size2 = resizeArea.height() - size1 - handle->height();

        if (size1 > 0 && size2 > 0) {
            handle->setY(handleY);

            resizeHandleItem1->setHeight(size1);
            resizeHandleItem2->setY(handle->y() + handle->height());
            resizeHandleItem2->setHeight(size2);
        }
    }
    return true;
}

bool QQuickDockLayoutPrivate::handleRelease(const QPointF &point, ulong timestamp)
{
    Q_UNUSED(point)
    Q_UNUSED(timestamp)

    updateSizeDataFromState();

    return true;
}

QQuickDockLayoutHandleAttachedPrivate::QQuickDockLayoutHandleAttachedPrivate()
    : m_hovered(false), m_pressed(false)
{
}

void QQuickDockLayoutHandleAttachedPrivate::setHovered(bool hovered)
{
    Q_Q(QQuickDockLayoutHandleAttached);
    if (hovered == m_hovered)
        return;

    m_hovered = hovered;
    emit q->hoveredChanged();
}

void QQuickDockLayoutHandleAttachedPrivate::setPressed(bool pressed)
{
    Q_Q(QQuickDockLayoutHandleAttached);
    if (pressed == m_pressed)
        return;

    m_pressed = pressed;
    emit q->pressedChanged();
}

QQuickDockLayoutHandleAttachedPrivate *
QQuickDockLayoutHandleAttachedPrivate::get(QQuickDockLayoutHandleAttached *attached)
{
    return attached->d_func();
}

const QQuickDockLayoutHandleAttachedPrivate *
QQuickDockLayoutHandleAttachedPrivate::get(const QQuickDockLayoutHandleAttached *attached)
{
    return attached->d_func();
}

QQuickDockLayout::QQuickDockLayout(QQuickItem *parent)
    : QQuickContainer{ *(new QQuickDockLayoutPrivate), parent }
{
    Q_D(QQuickDockLayout);
    d->changeTypes |= QQuickItemPrivate::Visibility;
    setFiltersChildMouseEvents(true);
}

void QQuickDockLayout::itemAdded(int index, QQuickItem *item)
{
    Q_D(QQuickDockLayout);
    Q_UNUSED(index);

    auto attached = qobject_cast<QQuickDockLayoutAttached *>(
            qmlAttachedPropertiesObject<QQuickDockLayout>(item, false));
    if (attached)
        QQuickDockLayoutAttachedPrivate::get(attached)->view = this;

    d->itemAdded(index, item);
}

void QQuickDockLayout::itemRemoved(int index, QQuickItem *item)
{
    Q_D(QQuickDockLayout);
    d->itemRemoved(index, item);
}

void QQuickDockLayout::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(QQuickDockLayout);
    QQuickContainer::hoverMoveEvent(event);

    QQuickItem *hoveredItem =
            childAt(event->position().toPoint().x(), event->position().toPoint().y());
    d->updateHoveredHandle(hoveredItem);
}

void QQuickDockLayout::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    Q_D(QQuickDockLayout);
    d->updateHoveredHandle(nullptr);
}

bool QQuickDockLayout::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    Q_D(QQuickDockLayout);

    if (Q_LIKELY(event->isPointerEvent())) {
        auto *pointerEvent = static_cast<QPointerEvent *>(event);

        const auto timestamp = pointerEvent->timestamp();
        const auto &eventPoint = pointerEvent->points().first();

        const QPointF point = mapFromItem(item, eventPoint.position());

        switch (event->type()) {
        case QEvent::MouseButtonPress:
            d->handlePress(point, timestamp);
            if (d->pressedHandleIndex != -1)
                item->setKeepMouseGrab(true);
            break;
        case QEvent::MouseButtonRelease:
            d->handleRelease(point, timestamp);
            break;
        case QEvent::MouseMove:
            if (d->pressedHandleIndex != -1)
                d->handleMove(point, timestamp);
            break;
        case QEvent::TouchBegin:
            if (pointerEvent->pointCount() == 1) {
                d->handlePress(point, timestamp);
                // We filter the event on behalf of item, but we want the item
                // to be the exclusive grabber so that we can continue to filter
                // touch events for it.
                if (d->pressedHandleIndex != -1) {
                    item->setKeepTouchGrab(true);
                    pointerEvent->setExclusiveGrabber(eventPoint, item);
                }
            }
            break;
        case QEvent::TouchEnd:
            if (pointerEvent->pointCount() == 1)
                d->handleRelease(point, timestamp);
            break;
        case QEvent::TouchUpdate:
            if (pointerEvent->pointCount() == 1)
                d->handleMove(point, timestamp);
            break;
        default:
            break;
        }
    }

    if (d->pressedHandleIndex != -1)
        return true;

    return QQuickContainer::childMouseEventFilter(item, event);
}

void QQuickDockLayout::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QQuickDockLayout);
    QQuickContainer::geometryChange(newGeometry, oldGeometry);

    d->layout();
}

bool QQuickDockLayout::isContent(QQuickItem *item) const
{
    Q_D(const QQuickDockLayout);
    if (!qmlContext(item))
        return false;

    if (QQuickItemPrivate::get(item)->isTransparentForPositioner())
        return false;

    return !d->handleItems.contains(item);
}

#if QT_CONFIG(accessibility)
QAccessible::Role QQuickDockLayout::accessibleRole() const
{
    return QAccessible::Pane;
}
#endif

Qt::Orientation QQuickDockLayout::orientation() const
{
    Q_D(const QQuickDockLayout);
    return d->orientation;
}

void QQuickDockLayout::setOrientation(Qt::Orientation newOrientation)
{
    Q_D(QQuickDockLayout);
    if (d->orientation == newOrientation)
        return;
    d->orientation = newOrientation;

#if QT_CONFIG(cursor)
    for (auto &handle : d->handleItems)
        handle->setCursor(newOrientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
#endif
    emit orientationChanged();
}

QQmlComponent *QQuickDockLayout::handle() const
{
    Q_D(const QQuickDockLayout);
    return d->handleComponent;
}

void QQuickDockLayout::setHandle(QQmlComponent *newHandle)
{
    Q_D(QQuickDockLayout);

    if (d->handleComponent == newHandle)
        return;
    d->handleComponent = newHandle;
    emit handleChanged();
}

QQuickDockLayoutAttached *QQuickDockLayout::qmlAttachedProperties(QObject *object)
{
    return new QQuickDockLayoutAttached(object);
}

void QQuickDockLayout::componentComplete()
{
    Q_D(QQuickDockLayout);
    d->ensureThereIsEnoughHandles();
    d->layout();

    QQuickContainer::componentComplete();
}

QQuickDockLayoutHandleAttached::QQuickDockLayoutHandleAttached(QObject *parent)
    : QObject(*(new QQuickDockLayoutHandleAttachedPrivate), parent)
{
}

bool QQuickDockLayoutHandleAttached::isHovered() const
{
    Q_D(const QQuickDockLayoutHandleAttached);
    return d->m_hovered;
}

bool QQuickDockLayoutHandleAttached::isPressed() const
{
    Q_D(const QQuickDockLayoutHandleAttached);
    return d->m_pressed;
}

QQuickDockLayoutHandleAttached *
QQuickDockLayoutHandleAttached::qmlAttachedProperties(QObject *object)
{
    return new QQuickDockLayoutHandleAttached(object);
}

QQuickDockLayoutAttachedPrivate::QQuickDockLayoutAttachedPrivate()
    : minimumSize{ -1 }, maximumSize{ std::numeric_limits<qreal>::infinity() }
{
}

QQuickDockLayoutAttached::QQuickDockLayoutAttached(QObject *parent)
    : QObject(*(new QQuickDockLayoutAttachedPrivate), parent)
{
}

QQuickDockLayout *QQuickDockLayoutAttached::view() const
{
    Q_D(const QQuickDockLayoutAttached);
    return d->view;
}

qreal QQuickDockLayoutAttached::minimumSize() const
{
    Q_D(const QQuickDockLayoutAttached);
    return d->minimumSize;
}

void QQuickDockLayoutAttached::setMinimumSize(qreal newMinimumSize)
{
    Q_D(QQuickDockLayoutAttached);
    if (qFuzzyCompare(d->minimumSize, newMinimumSize))
        return;
    d->minimumSize = newMinimumSize;
    emit minimumSizeChanged();
}

qreal QQuickDockLayoutAttached::maximumSize() const
{
    Q_D(const QQuickDockLayoutAttached);
    return d->maximumSize;
}

void QQuickDockLayoutAttached::setMaximumSize(qreal newMaximumSize)
{
    Q_D(QQuickDockLayoutAttached);
    if (qFuzzyCompare(d->maximumSize, newMaximumSize))
        return;
    d->maximumSize = newMaximumSize;
    emit maximumSizeChanged();
}

qreal QQuickDockLayoutAttached::preferredSize() const
{
    Q_D(const QQuickDockLayoutAttached);
    return d->preferredSize;
}

void QQuickDockLayoutAttached::setPreferredSize(qreal preferredSize)
{
    Q_D(QQuickDockLayoutAttached);

    if (qFuzzyCompare(d->preferredSize, preferredSize))
        return;
    d->preferredSize = preferredSize;
    emit preferredSizeChanged();
}

QT_END_NAMESPACE
