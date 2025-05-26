// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdocksidecontainer_p.h"
#include "qquickdocksidecontainer_p_p.h"
#include "qquickdockpanel_p.h"
#include "qquickcontentitem_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockSideContainer
    \inherits Container
    \instantiates QQuickDockSideContainer
    \inqmlmodule QtQuick.Controls
    \since 6.8
    \brief A container for dock widgets that supports one-way resizing.

 DockSideContainer is used internally by \l DockArea to hold a set of dock
 widgets aligned on a particular edge. It allows resizing in one direction,
 for example, the left area can expand or shrink horizontally towards the
 right.

 The resizeHandle property provides a visual and interactive handle for
 resizing, similar to the handle used by \l SplitView. This handle can be
 customized by the user.

 \section2 Properties

 \qmlproperty Item DockSideContainer::resizeHandle
 The item used as the resize handle to allow the user to adjust the container size.

 \qmlproperty enumeration DockSideContainer::resizeEdge
 The edge along which resizing is allowed. This uses the \c Qt::Edge
 enumeration (e.g., \c Qt::LeftEdge, \c Qt::RightEdge).

 \qmlproperty int DockSideContainer::size
 The current size (width or height depending on the resize edge) of the container.

 \sa DockArea, DockPanel
*/

QQuickItem *QQuickDockSideContainerPrivate::getContentItem()
{
    Q_Q(QQuickDockSideContainer);
    if (QQuickItem *item = QQuickContainerPrivate::getContentItem())
        return item;

    return new QQuickContentItem(q);
}

void QQuickDockSideContainerPrivate::addDockPanel(QQuickDockPanel *widget)
{
    dockPanels << widget;
    QQuickItemPrivate::get(widget)->addItemChangeListener(this, changeTypes);
}

void QQuickDockSideContainerPrivate::removeDockPanel(QQuickDockPanel *widget)
{
    Q_Q(QQuickDockSideContainer);
    dockPanels.removeOne(widget);
    emit q->panelRemoved(widget);
    // QQuickItemPrivate::get(widget)->removeItemChangeListener(this, changeTypes);
}

void QQuickDockSideContainerPrivate::updateSize()
{
    Q_Q(QQuickDockSideContainer);

    switch (resizeEdge) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        size = q->height();
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        size = q->width();
        break;
    }
}

void QQuickDockSideContainerPrivate::layout()
{
    auto content = getContentItem();
    // auto dockPanels = q->findChildren<QQuickDockPanel *>();
    // Q_ASSERT(this->dockPanels.size() == dockPanels.size());

    auto currentItem = dockPanels.value(currentIndex);
    switch (resizeEdge) {
    case Qt::TopEdge:
        resizeHandle->setX(0);
        resizeHandle->setY(0);
        resizeHandle->setWidth(content->width());
        resizeHandle->setHeight(resizeHandle->implicitHeight());

        for (auto &item : dockPanels) {
            item->setX(0);
            item->setY(resizeHandle->implicitHeight());
            item->setWidth(content->width());
            item->setHeight(content->height() - resizeHandle->implicitHeight());
            if (currentItem)
                item->setVisible(item == currentItem);
        }
        break;
    case Qt::LeftEdge:
        resizeHandle->setX(0);
        resizeHandle->setY(0);
        resizeHandle->setWidth(resizeHandle->implicitWidth());
        resizeHandle->setHeight(content->height());

        for (auto &item : dockPanels) {
            item->setX(resizeHandle->implicitWidth());
            item->setY(0);
            item->setWidth(content->width() - resizeHandle->implicitWidth());
            item->setHeight(content->height());
            if (currentItem)
                item->setVisible(item == currentItem);
        }
        break;
    case Qt::RightEdge:
        resizeHandle->setX(content->width() - resizeHandle->implicitWidth());
        resizeHandle->setY(0);
        resizeHandle->setWidth(resizeHandle->implicitWidth());
        resizeHandle->setHeight(content->height());

        for (auto &item : dockPanels) {
            item->setX(0);
            item->setY(0);
            item->setWidth(content->width() - resizeHandle->implicitWidth());
            item->setHeight(content->height());
            if (currentItem)
                item->setVisible(item == currentItem);
        }
        break;
    case Qt::BottomEdge:
        resizeHandle->setX(0);
        resizeHandle->setY(content->height() - resizeHandle->implicitHeight());
        resizeHandle->setWidth(content->width());
        resizeHandle->setHeight(resizeHandle->implicitHeight());

        for (auto &item : dockPanels) {
            item->setX(0);
            item->setY(0);
            item->setWidth(content->width());
            item->setHeight(content->height() - resizeHandle->implicitHeight());
            if (currentItem)
                item->setVisible(item == currentItem);
        }
        break;
    }
}

bool QQuickDockSideContainerPrivate::handlePress(const QPointF &point, ulong timestamp)
{
    // TODO call base class impl or explain why not
    Q_UNUSED(timestamp);
    updateSize();
    clickPos = point;
    startSize = size;
    return true; // TODO
}

bool QQuickDockSideContainerPrivate::handleMove(const QPointF &point, ulong timestamp)
{
    // TODO call base class impl or explain why not
    Q_UNUSED(timestamp);
    Q_Q(QQuickDockSideContainer);

    auto diff = point - clickPos;

    switch (resizeEdge) {
    case Qt::BottomEdge:
        size = startSize + diff.y();
        break;
    case Qt::RightEdge:
        size = startSize + diff.x();
        break;
    case Qt::LeftEdge:
        size = startSize - diff.x();
        break;
    case Qt::TopEdge:
        size = startSize - diff.y();
        break;
    }
    emit q->sizeChanged();
    return true; // TODO
}

void QQuickDockSideContainerPrivate::updateResizeHandlerCursor()
{
    switch (resizeEdge) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        resizeHandle->setCursor(Qt::SplitVCursor);
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        resizeHandle->setCursor(Qt::SplitHCursor);
        break;
    }
}

void QQuickDockSideContainerPrivate::updatePolish()
{
    layout();
}

void QQuickDockSideContainerPrivate::itemParentChanged(QQuickItem *item, QQuickItem *parent)
{
    Q_UNUSED(parent)

    auto panel = qobject_cast<QQuickDockPanel *>(item);
    if (panel)
        removeDockPanel(panel);
}

QQuickDockSideContainerAttachedPrivate::QQuickDockSideContainerAttachedPrivate()
    : m_hovered(false), m_pressed(false)
{
}

QQuickDockSideContainerAttachedPrivate *
QQuickDockSideContainerAttachedPrivate::get(QQuickDockSideContainerAttached *attached)
{
    return attached->d_func();
}

const QQuickDockSideContainerAttachedPrivate *
QQuickDockSideContainerAttachedPrivate::get(const QQuickDockSideContainerAttached *attached)
{
    return attached->d_func();
}

void QQuickDockSideContainerAttachedPrivate::setHovered(bool hovered)
{
    Q_Q(QQuickDockSideContainerAttached);
    if (hovered == m_hovered)
        return;

    m_hovered = hovered;
    emit q->hoveredChanged();
}

void QQuickDockSideContainerAttachedPrivate::setPressed(bool pressed)
{
    Q_Q(QQuickDockSideContainerAttached);
    if (pressed == m_pressed)
        return;

    m_pressed = pressed;
    emit q->pressedChanged();
}

QQuickDockSideContainer::QQuickDockSideContainer(QQuickItem *parent)
    : QQuickContainer(*(new QQuickDockSideContainerPrivate), parent)
{
    setAcceptHoverEvents(true);
    setFiltersChildMouseEvents(true);
    Q_D(QQuickDockSideContainer);
    d->resizeHandle = new QQuickItem{ this };
    QObjectPrivate::connect(this, &QQuickContainer::currentIndexChanged, d,
                            &QQuickDockSideContainerPrivate::layout);
}

void QQuickDockSideContainer::componentComplete()
{
    Q_D(QQuickDockSideContainer);
    QQuickContainer::componentComplete();
    switch (d->resizeEdge) {
    case Qt::TopEdge:
    case Qt::BottomEdge:
        d->size = implicitHeight();
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        d->size = implicitWidth();
        break;
    }
}

QQuickItem *QQuickDockSideContainer::resizeHandle() const
{
    Q_D(const QQuickDockSideContainer);
    return d->resizeHandle;
}

void QQuickDockSideContainer::setResizeHandle(QQuickItem *newResizeHandle)
{
    Q_D(QQuickDockSideContainer);
    if (d->resizeHandle == newResizeHandle)
        return;
    d->resizeHandle = newResizeHandle;
    newResizeHandle->setAcceptedMouseButtons(Qt::LeftButton);
    newResizeHandle->setKeepMouseGrab(true);
    newResizeHandle->setAcceptHoverEvents(true);
    d->updateResizeHandlerCursor();

    d->handleAttached = qobject_cast<QQuickDockSideContainerAttached *>(
            qmlAttachedPropertiesObject<QQuickDockSideContainerAttached>(newResizeHandle, true));
    d->handleAttachedPrivate = QQuickDockSideContainerAttachedPrivate::get(d->handleAttached);

    addItem(newResizeHandle);
    emit resizeHandleChanged();
}

void QQuickDockSideContainer::itemAdded(int index, QQuickItem *item)
{
    Q_UNUSED(index);

    Q_D(QQuickDockSideContainer);

    auto panel = qobject_cast<QQuickDockPanel *>(item);
    if (panel)
        d->addDockPanel(panel);
    d->layout();
}

void QQuickDockSideContainer::itemRemoved(int index, QQuickItem *item)
{
    Q_UNUSED(index);
    Q_D(QQuickDockSideContainer);

    auto panel = qobject_cast<QQuickDockPanel *>(item);
    if (panel)
        d->removeDockPanel(panel);
}

bool QQuickDockSideContainer::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    Q_D(QQuickDockSideContainer);

    if (item != d->resizeHandle)
        return QQuickContainer::childMouseEventFilter(item, event);

    auto me = static_cast<QMouseEvent *>(event);
    switch (event->type()) {
    case QEvent::HoverMove:
        d->handleAttachedPrivate->setHovered(true);
        break;
    case QEvent::HoverLeave:
        d->handleAttachedPrivate->setHovered(false);
        break;
    case QEvent::MouseButtonPress:
        me->accept();
        d->handlePress(me->scenePosition(), me->timestamp());
        d->handleAttachedPrivate->setPressed(true);
        break;

    case QEvent::MouseMove: {
        d->handleMove(me->scenePosition(), me->timestamp());
        break;
    }

    case QEvent::MouseButtonRelease:
        d->handleAttachedPrivate->setPressed(false);
        break;

    default:
        break;
    }

    return true;
}

void QQuickDockSideContainer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickContainer::geometryChange(newGeometry, oldGeometry);
    polish();
}

void QQuickDockSideContainer::hoverMoveEvent(QHoverEvent *event)
{
    QQuickContainer::hoverMoveEvent(event);

    // QQuickItem *hoveredItem = contentItem()->childAt(event->position().toPoint().x(),
    //                                                  event->position().toPoint().y());
    // if (hoveredItem == d->resizeHandle)
    //     d->handleAttachedPrivate->setHovered(true);
}

void QQuickDockSideContainer::hoverLeaveEvent(QHoverEvent *event)
{
    QQuickContainer::hoverMoveEvent(event);

    // QQuickItem *hoveredItem = contentItem()->childAt(event->position().toPoint().x(),
    //                                                  event->position().toPoint().y());
    // if (hoveredItem == d->resizeHandle)
    //     d->handleAttachedPrivate->setHovered(false);
}

Qt::Edge QQuickDockSideContainer::resizeEdge() const
{
    Q_D(const QQuickDockSideContainer);
    return d->resizeEdge;
}

void QQuickDockSideContainer::setResizeEdge(Qt::Edge newResizeEdge)
{
    Q_D(QQuickDockSideContainer);

    if (d->resizeEdge == newResizeEdge)
        return;
    d->resizeEdge = newResizeEdge;
    d->updateResizeHandlerCursor();
    emit resizeEdgeChanged();
}

int QQuickDockSideContainer::size() const
{
    Q_D(const QQuickDockSideContainer);
    return d->size;
}

void QQuickDockSideContainer::setSize(int newSize)
{
    Q_D(QQuickDockSideContainer);

    if (d->size == newSize)
        return;
    d->size = newSize;
    emit sizeChanged();
}

QQuickDockSideContainerAttached::QQuickDockSideContainerAttached(QObject *parent)
    : QObject(*(new QQuickDockSideContainerAttachedPrivate), parent)
{
}

bool QQuickDockSideContainerAttached::isHovered() const
{
    Q_D(const QQuickDockSideContainerAttached);
    return d->m_hovered;
}

bool QQuickDockSideContainerAttached::isPressed() const
{
    Q_D(const QQuickDockSideContainerAttached);
    return d->m_pressed;
}

QQuickDockSideContainerAttached *
QQuickDockSideContainerAttached::qmlAttachedProperties(QObject *object)
{
    return new QQuickDockSideContainerAttached(object);
}

QT_END_NAMESPACE

#include "moc_qquickdocksidecontainer_p.cpp"
