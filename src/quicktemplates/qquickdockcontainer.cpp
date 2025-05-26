// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>

#include "qquickdockcontainer_p.h"
#include "qquickdockcontainer_p_p.h"
#include "qquickdockmoveguide_p.h"
#include "qquickdockpanel_p.h"
#include "qquickdockarea_p.h"
#include "qquickdockarea_p.h"
#include "qquickdocklayout_p.h"
#include "qquickdocklayout_p.h"
#include "qquickdockgroup_p.h"
#include "qquickdockpanel_p_p.h"
#include "qquickdockmoveguide_p_p.h"
#include "qquickcontentitem_p.h"
#include "qquickdockgroup_p_p.h"
#include "qquickdockarea_p_p.h"
#include "qquickdocklayout_p_p.h"
#include "qquickdocktabbar_p.h" // tmp
#include "qquickdocksidecontainer_p.h" // tmp

#include <QDebug>
#include <QPainter>
#include <QSettings>

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockContainer
    \inherits Container
//!    \instantiates QQuickDockContainer
    \inqmlmodule QtQuick.Controls
    \since 6.8
    \ingroup qtquickcontrols-docks
    \brief Provides a container for managing dockable widgets in a QML application.

 DockContainer is the main component for building complex dockable UIs in QML.
 It manages the layout, visibility, and interaction of multiple \l DockPanel
 instances. This includes support for four dock areas (top, bottom, left, right),
 drag-and-drop docking, tab grouping, and detachable panels.

 The container handles layout rules and manages \l DockArea, \l DockPanelGroup,
 and layout splitters internally.

 A typical application will use a single DockContainer instance as the main
 UI container.

 \qml
 DockContainer {
     anchors.fill: parent
     DockPanel {
         title: "Project"
         contentItem: ProjectTree {}
     }
     DockPanel {
         title: "Editor"
         contentItem: CodeEditor {}
     }
 }
 \endqml
*/

/*!
    \qmlproperty list<DockPanel> DockContainer::dockPanels
    \readonly

 Holds the list of all dock widgets currently managed by this container.
*/

/*!
    \qmlproperty DockPanel DockContainer::activeWidget
    \readonly

 This property holds the currently active (visible) dock widget among all grouped ones.
*/

/*!
    \qmlproperty DockArea DockContainer::topArea
    \qmlproperty DockArea DockContainer::bottomArea
    \qmlproperty DockArea DockContainer::leftArea
    \qmlproperty DockArea DockContainer::rightArea

 These properties hold the \l DockArea components that manage widgets docked
 to each edge of the container.

 Normally, these areas are created and managed internally, but they can be
 overridden for customization.
*/

/*!
    \qmlproperty Item DockContainer::dropArea

 This property holds the visual item that indicates where a widget can be
 dropped when dragging.

 You can override this to provide a custom visual representation.
*/

/*!
    \qmlproperty DockMoveGuide DockContainer::moveGuide

 This property holds the move guide helper object that displays overlays during
 drag operations.

 You can assign a custom guide component here for a different UI appearance.
*/

/*!
    \qmlproperty Component DockContainer::splitView

 This property holds the component used to construct the layout splitter
 between docked areas.

 A default splitter is provided, but you can override it for a customized look.
*/

/*!
    \qmlproperty Component DockContainer::widgetsGroup

 This property holds the component used to render groups of dock widgets
 (i.e., tabbed panels).

 Override this to customize the appearance or behavior of tab groups.
*/

/*!
    \qmlproperty bool DockContainer::popupType


*/

/*!
    \qmlattachedtype DockContainer

 Provides attached properties to customize the behavior of individual
 DockPanels inside a DockContainer.

 \sa DockPanel
*/

/*!
    \qmlattachedproperty enumeration DockContainer::edge

 Specifies which edge the dock widget prefers to be attached to.

 \value Qt.TopEdge
 \value Qt.BottomEdge
 \value Qt.LeftEdge
 \value Qt.RightEdge

 You can set this property to give the widget an initial docking position.
*/

namespace {

QString prettyName(QQuickItem *item)
{
    if (auto area = qobject_cast<QQuickDockArea *>(item)) {
        QString edge;
        switch (area->tabPosition()) {
        case Qt::TopEdge:
            edge = QStringLiteral("Top");
            break;
        case Qt::LeftEdge:
            edge = QStringLiteral("Left");
            break;
        case Qt::RightEdge:
            edge = QStringLiteral("Right");
            break;
        case Qt::BottomEdge:
            edge = QStringLiteral("Bottom");
            break;
        }
        return QQmlMetaType::prettyTypeName(item) + QStringLiteral(" ") + edge;
    }

    if (qobject_cast<QQuickDockContainer *>(item) || qobject_cast<QQuickDockPanel *>(item)
        || qobject_cast<QQuickDockGroup *>(item) || qobject_cast<QQuickDockSideContainer *>(item))
        return QQmlMetaType::prettyTypeName(item);

    return {};
}
void dumpChildren(QQuickItem *item, int level = 0)
{
    auto name = prettyName(item);
    if (!name.isNull()) {
        QString space{ level * 3, QLatin1Char(' ') };
        qDebug().noquote().nospace() << space << "* " << prettyName(item);
    }

    if (qobject_cast<QQuickDockMoveGuide *>(item))
        return;
    if (qobject_cast<QQuickDockPanel *>(item))
        return;
    if (qobject_cast<QQuickDockTabBar *>(item))
        return;

    auto children = item->childItems();
    for (auto &child : children)
        dumpChildren(child, level + (name.isNull() ? 0 : 1));
}

namespace z {
constexpr int group{ 100 };
constexpr int widget{ 200 };
// constexpr int floatWidget{ 300 };
// constexpr int resizer{ 400 };
// constexpr int guide{ 500 };

} // namespace z
} // namespace

QQuickItem *QQuickDockContainerPrivate::getContentItem()
{
    Q_Q(QQuickDockContainer);
    if (QQuickItem *item = QQuickContainerPrivate::getContentItem())
        return item;

    return new QQuickContentItem(q);
}

QQuickDockArea *QQuickDockContainerPrivate::area(Qt::Edge edge) const
{
    switch (edge) {
    case Qt::TopEdge:
        return topArea;
    case Qt::LeftEdge:
        return leftArea;
    case Qt::RightEdge:
        return rightArea;
    case Qt::BottomEdge:
        return bottomArea;
    }
    return nullptr;
}

QQuickDockLayout *QQuickDockContainerPrivate::createSplitView(QQuickItem *parent)
{
    Q_Q(QQuickDockContainer);
    if (!splitViewCreationComponent)
        return nullptr;

    auto context = QQmlEngine::contextForObject(splitViewCreationComponent); // Get a valid context
    if (!context) {
        qDebug() << "Failed to get a valid QQmlContext!";
        return nullptr;
    }

    auto obj = splitViewCreationComponent->create(context);
    auto view = qobject_cast<QQuickDockLayout *>(obj);
    view->setParentItem(parent);
    QQuickDockLayoutPrivate::get(view)->container = q;
    return view;
}

QQuickDockGroup *QQuickDockContainerPrivate::createGroup(QQuickItem *parent)
{
    Q_Q(QQuickDockContainer);

    // QQmlContext *context = widgetsGroupCreationComponent->creationContext();

    // QQuickItem *groupItem =
    //         qobject_cast<QQuickItem *>(widgetsGroupCreationComponent->beginCreate(context));

    // if (groupItem) {
    //     groupItem->setParentItem(parent);
    //     return qobject_cast<QQuickDockGroup*>(groupItem);
    // }
    // return nullptr;
    if (!widgetsGroupCreationComponent)
        return nullptr;

    auto context =
            QQmlEngine::contextForObject(widgetsGroupCreationComponent); // Get a valid context
    if (!context) {
        qDebug() << "Failed to get a valid QQmlContext!";
        return nullptr;
    }

    auto obj = widgetsGroupCreationComponent->create(context);
    auto view = qobject_cast<QQuickDockGroup *>(obj);
    view->setParentItem(parent);

    auto dd = QQuickDockGroupPrivate::get(view);
    dd->dockContainer = q;
    // dd->autoDelete = true;
    view->setZ(z::group);
    return view;
}

void QQuickDockContainerPrivate::init(QQuickDockLayout *layout)
{
    Q_Q(QQuickDockContainer);
    auto d = QQuickDockLayoutPrivate::get(layout);
    d->container = q;
    for (int i = 0; i < d->contentModel->count(); ++i) {
        auto item = d->itemAt(i);

        auto layout = qobject_cast<QQuickDockLayout *>(item);
        if (layout)
            init(layout);

        auto group = qobject_cast<QQuickDockGroup *>(item);
        if (group)
            init(group);
    }
}

void QQuickDockContainerPrivate::changeDockArea(QQuickDockArea **oldArea, QQuickDockArea *newArea)
{
    Q_Q(QQuickDockContainer);

    if (*oldArea) {
        disconnect(*oldArea, &QQuickDockArea::countChanged, this,
                   &QQuickDockContainerPrivate::areaCountChanged);
    }
    *oldArea = newArea;

    addItem(newArea);
    QQuickDockAreaPrivate::get(newArea)->dockContainer = q;
    connect(newArea, &QQuickDockArea::countChanged, this,
            &QQuickDockContainerPrivate::areaCountChanged);
}

void QQuickDockContainerPrivate::init(QQuickDockGroup *group)
{
    Q_Q(QQuickDockContainer);

    auto d = QQuickDockGroupPrivate::get(group);
    d->dockContainer = q;

    for (int i = 0; i < group->count(); ++i) {
        auto widget = qobject_cast<QQuickDockPanel *>(group->itemAt(i));
        if (widget) {
            QQuickDockPanelPrivate::get(widget)->group = group;
            init(widget);
        }
    }
}

void QQuickDockContainerPrivate::init(QQuickDockPanel *widget)
{
    Q_Q(QQuickDockContainer);
    auto d = QQuickDockPanelPrivate::get(widget);
    d->dockContainer = q;
    widget->setZ(z::widget);
    connectWidgetSignals(widget);

    switch (d->visibility) {

    case QQuickDockPanel::Closed:
    case QQuickDockPanel::Unpinned:
    case QQuickDockPanel::Pinned:
    case QQuickDockPanel::Detached:
        break;
    }
}

void QQuickDockContainerPrivate::addWidget(QQuickDockPanel *widget, Qt::Edge edge)
{
    auto p = QQuickDockPanelPrivate::get(widget);
    p->attach();
    switch (edge) {
    case Qt::TopEdge:
        topArea->addDockPanel(widget);
        break;
    case Qt::LeftEdge:
        leftArea->addDockPanel(widget);
        break;
    case Qt::RightEdge:
        rightArea->addDockPanel(widget);
        break;
    case Qt::BottomEdge:
        bottomArea->addDockPanel(widget);
        break;
    }
}

void QQuickDockContainerPrivate::setPopupType(QQuickDockContainer::PopupType type)
{
    popupType = type;
    allowDetach = (type == QQuickDockContainer::PopupType::Native
                   || type == QQuickDockContainer::PopupType::Window)
            && QGuiApplicationPrivate::platformIntegration()->hasCapability(
                    QPlatformIntegration::Capability::MultipleWindows);
}

void QQuickDockContainerPrivate::storeAllDropAreas()
{
    Q_Q(QQuickDockContainer);

    dropAreas.clear();
    QQuickDockLayoutPrivate::get(rootSplitView)->iterateForDropArea(q, dropAreas);
}

void QQuickDockContainerPrivate::layout()
{
    // topArea->setVisible(false);
    // leftArea->setVisible(false);
    // rightArea->setVisible(false);
    // bottomArea->setVisible(false);

    topArea->setZ(2);
    bottomArea->setZ(2);
    leftArea->setZ(2);
    rightArea->setZ(2);

    topArea->setVisible(topArea->count());
    bottomArea->setVisible(bottomArea->count());
    leftArea->setVisible(leftArea->count());
    rightArea->setVisible(rightArea->count());

    contentRect.setCoords(
            leftArea->isVisible() ? leftArea->width() : 0,
            topArea->isVisible() ? topArea->height() : 0,
            getContentItem()->width() - (rightArea->isVisible() ? rightArea->width() : 0),
            getContentItem()->height() - (bottomArea->isVisible() ? bottomArea->height() : 0));

    if (leftArea->isVisible()) {
        leftArea->setY(contentRect.y());
        leftArea->setHeight(contentRect.height());
    }

    if (topArea->isVisible()) {
        topArea->setX(contentRect.x());
        topArea->setWidth(contentRect.width());
    }

    if (rightArea->isVisible()) {
        rightArea->setX(contentRect.right());
        rightArea->setY(contentRect.y());
        rightArea->setHeight(contentRect.height());
    }

    if (bottomArea->isVisible()) {
        bottomArea->setX(contentRect.x());
        bottomArea->setY(contentRect.bottom());
        bottomArea->setWidth(contentRect.width());
    }

    // leftArea->setWidth(leftArea->usedSize());
    if (rootSplitView) {
        rootSplitView->setPosition(contentRect.topLeft());
        rootSplitView->setSize(contentRect.size());
        rootSplitView->setZ(1);
    }
}

void QQuickDockContainerPrivate::addItem(QQuickItem *item)
{
    Q_Q(QQuickDockContainer);
    // item->setParentItem(contentItem);
    q->addItem(item);
}

void QQuickDockContainerPrivate::updatePolish()
{
    layout();
}

void QQuickDockContainerPrivate::setActiveWidget(QQuickDockPanel *widget)
{
    Q_Q(QQuickDockContainer);

    if (activePanel == widget)
        return;

    for (auto &widget : dockPanels)
        widget->setZ(std::max(widget->z() - 1, zFloatingWidgetStart));

    if (activePanel) {
        // auto ddOld = QQuickDockPanelPrivate::get(activeWidget);
        // ddOld->setActive(false);
    }
    activePanel = widget;

    if (widget) {
        auto ddNew = QQuickDockPanelPrivate::get(widget);
        ddNew->setActive(true);
        widget->setZ(zFloatingWidgetEnd);
    }

    Q_EMIT q->activeWidgetChanged();
    /*
        auto p = widget->parentItem();
        while (p) {
            auto g = qobject_cast<QQuickDockGroup *>(p);

            if (g) {
                auto groupPrivate = QQuickDockGroupPrivate::get(g);

                groupPrivate->setActive(true);
                return;
            }
            p = p->parentItem();
        }*/
}

void QQuickDockContainerPrivate::setActiveItem(QQuickItem *item)
{
    while (item) {
        auto dw = qobject_cast<QQuickDockPanel *>(item);

        if (dw) {
            setActiveWidget(dw);
            return;
        }
        item = item->parentItem();
    }
}

void QQuickDockContainerPrivate::createDefaultLayout()
{
    rootSplitView = createSplitView(contentItem);
    rootSplitView->setOrientation(Qt::Vertical);
    rootSplitView->setVisible(true);
    if (!rootSplitView)
        qFatal() << "Unable to create the root view";

    auto g = createGroup(nullptr); // d->rootSplitView->contentItem());
    g->setVisible(true);
    g->setSize(QSize{ 200, 200 });
    rootSplitView->addItem(g);

    addItem(rootSplitView);
}

QQuickDockContainer::PopupType QQuickDockContainerPrivate::resolvedPopupType() const
{
    if (popupType == QQuickDockContainer::PopupType::Window
        || popupType == QQuickDockContainer::PopupType::Native) {
        if (QGuiApplicationPrivate::platformIntegration()->hasCapability(
                    QPlatformIntegration::Capability::MultipleWindows))
            return QQuickDockContainer::PopupType::Window;
    }

    return QQuickDockContainer::PopupType::Item;
}

bool QQuickDockContainerPrivate::usePopupWindow() const
{
    return resolvedPopupType() == QQuickDockContainer::PopupType::Window;
}

void QQuickDockContainerPrivate::connectWidgetSignals(QQuickDockPanel *widget)
{
    Q_Q(QQuickDockContainer);

    if (dockPanels.contains(widget)) {
        return;
    }
    auto dd = QQuickDockPanelPrivate::get(widget);
    dd->dockContainer = q;
    // dd->isOpen = true;

    widget->setParent(q);

    // widget->setZ(z::widget);
    dockPanels.append(widget);

    QObjectPrivate::connect(
            widget, &QQuickDockPanel::beginMove, this,
            &QQuickDockContainerPrivate::dockPanelBeginMove,
            static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
    QObjectPrivate::connect(widget, &QQuickDockPanel::moving, this,
                            &QQuickDockContainerPrivate::dockPanelMoving, Qt::UniqueConnection);
    QObjectPrivate::connect(widget, &QQuickDockPanel::moved, this,
                            &QQuickDockContainerPrivate::dockPanelMoved, Qt::UniqueConnection);
    QObjectPrivate::connect(widget, &QQuickItem::visibleChanged, this,
                            &QQuickDockContainerPrivate::dockPanelVisibleChanged,
                            Qt::UniqueConnection);

    Q_EMIT q->dockPanelsChanged(dockPanels);
}

void QQuickDockContainerPrivate::dockPanelBeginMove()
{
    Q_Q(QQuickDockContainer);
    storeAllDropAreas();
    currentDropArea = dropAreas.end();
    dockMoveGuide->begin();
    auto panel = qobject_cast<QQuickDockPanel *>(q->sender());
    if (panel) {
        auto panelPrivate = QQuickDockPanelPrivate::get(panel);
        if (panelPrivate->group)
            QQuickDockGroupPrivate::get(panelPrivate->group)->removeDockPanel(panel);
    }
}

void QQuickDockContainerPrivate::dockPanelMoving(const QPointF &pt)
{
    Q_Q(QQuickDockContainer);

    auto localPos = q->mapFromGlobal(pt);

    if (currentDropArea == dropAreas.end() || !currentDropArea->rect.contains(localPos)) {
        currentDropArea =
                std::find_if(dropAreas.begin(), dropAreas.end(),
                             [&localPos](const QQuickDockLayoutPrivate::DropAreaData &area) {
                                 return area.rect.contains(localPos);
                             });

        if (currentDropArea != dropAreas.end()) {
            dockMoveGuide->setAreaRect(currentDropArea->rect);
            dockMoveGuide->setAllowFill(currentDropArea->areaType
                                        == QQuickDockLayoutPrivate::DropAreaData::AreaType::Group);
        }
    }

    dockMoveGuide->setMousePos(localPos);

    if (dockMoveGuide->currentSide() == QQuickDockMoveGuide::None) {
        dockIndicatorItem->setVisible(false);
    } else {
        dockIndicatorItem->setVisible(true);
        dockIndicatorItem->setPosition(dockMoveGuide->highlightRect().topLeft());
        dockIndicatorItem->setSize(dockMoveGuide->highlightRect().size());
    }
}

void QQuickDockContainerPrivate::dockPanelMoved()
{
    Q_Q(QQuickDockContainer);
    dockMoveGuide->end();
    dockMoveGuide->setVisible(false);
    dockIndicatorItem->setVisible(false);

    auto dw = qobject_cast<QQuickDockPanel *>(q->sender());
    if (!dw)
        return;

    dumpChildren(q);
    switch (dockMoveGuide->currentSide()) {
    case QQuickDockMoveGuide::None:
        break;
    case QQuickDockMoveGuide::WindowTop:
        addWidget(dw, Qt::TopEdge);
        break;
    case QQuickDockMoveGuide::WindowBottom:
        addWidget(dw, Qt::BottomEdge);
        break;
    case QQuickDockMoveGuide::WindowLeft:
        addWidget(dw, Qt::LeftEdge);
        break;
    case QQuickDockMoveGuide::WindowRight:
        addWidget(dw, Qt::RightEdge);
        break;
    case QQuickDockMoveGuide::Fill: {
        auto group = qobject_cast<QQuickDockGroup *>(currentDropArea->item);
        if (group)
            group->addItem(dw);
        // dw->pinTo(group);
        // group->addDockPanel(dw);
        break;
    }
    case QQuickDockMoveGuide::PanelTop:
        QQuickDockLayoutPrivate::get(currentDropArea->layout)
                ->addDockPanel(currentDropArea->item, dw, Qt::TopEdge);
        // addWidget(currentLayout, dw, Qt::TopEdge);
        break;
    case QQuickDockMoveGuide::PanelBottom:
        // addWidget(currentLayout, dw, Qt::BottomEdge);
        QQuickDockLayoutPrivate::get(currentDropArea->layout)
                ->addDockPanel(currentDropArea->item, dw, Qt::BottomEdge);
        break;
    case QQuickDockMoveGuide::PanelLeft:
        // addWidget(currentLayout, dw, Qt::LeftEdge);
        QQuickDockLayoutPrivate::get(currentDropArea->layout)
                ->addDockPanel(currentDropArea->item, dw, Qt::LeftEdge);
        break;
    case QQuickDockMoveGuide::PanelRight:
        // addWidget(currentLayout, dw, Qt::RightEdge);
        QQuickDockLayoutPrivate::get(currentDropArea->layout)
                ->addDockPanel(currentDropArea->item, dw, Qt::RightEdge);
        break;
    }

    layout();
}

void QQuickDockContainerPrivate::dockPanelVisibleChanged() { }

void QQuickDockContainerPrivate::windowActiveFocusItemChanged()
{
    setActiveItem(window->activeFocusItem());
}

void QQuickDockContainerPrivate::areaCountChanged()
{
    layout();
}

QQuickDockContainer::QQuickDockContainer(QQuickItem *parent)
    : QQuickContainer{ *(new QQuickDockContainerPrivate), parent }
{
    QQuickWindow::setDefaultAlphaBuffer(true);

    setFiltersChildMouseEvents(true);
}

void QQuickDockContainer::componentComplete()
{
    Q_D(QQuickDockContainer);
    QQuickContainer::componentComplete();

    if (!d->rootSplitView) {
        d->createDefaultLayout();
    }

    if (window())
        QObjectPrivate::connect(window(), &QQuickWindow::activeFocusItemChanged, d,
                                &QQuickDockContainerPrivate::windowActiveFocusItemChanged);

    d->dockPanels.clear();

    d->area(Qt::LeftEdge)->close();
    d->area(Qt::RightEdge)->close();
    d->area(Qt::TopEdge)->close();
    d->area(Qt::BottomEdge)->close();
    d->updatePolish();
}

QList<QQuickDockPanel *> QQuickDockContainer::dockPanels() const
{
    Q_D(const QQuickDockContainer);
    return d->dockPanels;
}

void QQuickDockContainer::removeDockPanel(QQuickDockPanel *widget)
{
    // if (widget->dockArea())
    // widget->dockArea()->removeDockPanel(widget);
    widget->setParentItem(nullptr);
    // d->removedDockPanels.append(widget);
}

void QQuickDockContainer::itemAdded(int index, QQuickItem *item)
{
    Q_UNUSED(index)
    Q_UNUSED(item)
    Q_D(QQuickDockContainer);

    auto layout = qobject_cast<QQuickDockLayout *>(item);
    if (layout) {
        d->rootSplitView = layout;
        d->init(layout);

        auto ws = layout->findChildren<QQuickDockPanel *>();

        for (auto &w : ws) {
            d->connectWidgetSignals(w);
        }

        auto layouts = layout->findChildren<QQuickDockLayout *>();
        for (auto &l : layouts)
            QQuickDockLayoutPrivate::get(l)->container = this;

        auto groups = layout->findChildren<QQuickDockGroup *>();
        for (auto &g : groups)
            QQuickDockGroupPrivate::get(g)->dockContainer = this;
    }

    /*auto group = qobject_cast<QQuickDockGroup *>(item);
    if (group) {
        QQuickDockGroupPrivate::get(group)->dockContainer = this;
        auto ws = layout->findChildren<QQuickDockPanel *>();
        for (auto &w : ws)
            d->connectWidgetSignals(w);
    }*/
}

bool QQuickDockContainer::childMouseEventFilter(QQuickItem *item, QEvent *event)
{
    return QQuickContainer::childMouseEventFilter(item, event);
}

void QQuickDockContainer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QQuickDockContainer);
    QQuickContainer::geometryChange(newGeometry, oldGeometry);

    d->layout();
}

QQuickDockArea *QQuickDockContainer::topArea() const
{
    Q_D(const QQuickDockContainer);
    return d->topArea;
}

void QQuickDockContainer::setTopArea(QQuickDockArea *newTopArea)
{
    Q_D(QQuickDockContainer);
    if (d->topArea == newTopArea)
        return;

    d->changeDockArea(&d->topArea, newTopArea);

    emit topAreaChanged();
}

QQuickDockArea *QQuickDockContainer::bottomArea() const
{
    Q_D(const QQuickDockContainer);
    return d->bottomArea;
}

void QQuickDockContainer::setBottomArea(QQuickDockArea *newBottomArea)
{
    Q_D(QQuickDockContainer);
    if (d->bottomArea == newBottomArea)
        return;
    d->changeDockArea(&d->bottomArea, newBottomArea);
    emit bottomAreaChanged();
}

QQuickDockArea *QQuickDockContainer::rightArea() const
{
    Q_D(const QQuickDockContainer);
    return d->rightArea;
}

void QQuickDockContainer::setRightArea(QQuickDockArea *newArea)
{
    Q_D(QQuickDockContainer);
    if (d->rightArea == newArea)
        return;
    d->changeDockArea(&d->rightArea, newArea);
    emit rightAreaChanged();
}

QQuickDockArea *QQuickDockContainer::leftArea() const
{
    Q_D(const QQuickDockContainer);
    return d->leftArea;
}

void QQuickDockContainer::setLeftArea(QQuickDockArea *newArea)
{
    Q_D(QQuickDockContainer);
    if (d->leftArea == newArea)
        return;
    d->changeDockArea(&d->leftArea, newArea);
    emit leftAreaChanged();
}

QQuickItem *QQuickDockContainer::dropArea() const
{
    Q_D(const QQuickDockContainer);

    return d->dropArea;
}

void QQuickDockContainer::setDropArea(QQuickItem *newDropArea)
{
    Q_D(QQuickDockContainer);
    if (d->dropArea == newDropArea)
        return;

    d->addItem(newDropArea);
    d->dropArea = newDropArea;
    emit dropAreaChanged();
}

QQuickDockMoveGuide *QQuickDockContainer::moveGuide() const
{
    Q_D(const QQuickDockContainer);
    return d->dockMoveGuide;
}

void QQuickDockContainer::setMoveGuide(QQuickDockMoveGuide *newMoveGuide)
{
    Q_D(QQuickDockContainer);
    if (d->dockMoveGuide == newMoveGuide)
        return;
    d->addItem(newMoveGuide);
    newMoveGuide->setZ(d->zMoveGuide);
    d->dockMoveGuide = newMoveGuide;
    QQuickDockMoveGuidePrivate::get(newMoveGuide)->parentDockContainer = this;
    emit moveGuideChanged();
}

QQmlComponent *QQuickDockContainer::splitView() const
{
    Q_D(const QQuickDockContainer);
    return d->splitViewCreationComponent;
}

void QQuickDockContainer::setSplitView(QQmlComponent *newSplitView)
{
    Q_D(QQuickDockContainer);
    if (d->splitViewCreationComponent == newSplitView)
        return;
    d->splitViewCreationComponent = newSplitView;
    emit splitViewChanged();
}

QQmlComponent *QQuickDockContainer::widgetsGroup() const
{
    Q_D(const QQuickDockContainer);
    return d->widgetsGroupCreationComponent;
}

void QQuickDockContainer::setWidgetsGroup(QQmlComponent *newWidgetsGroup)
{
    Q_D(QQuickDockContainer);
    if (d->widgetsGroupCreationComponent == newWidgetsGroup)
        return;
    d->widgetsGroupCreationComponent = newWidgetsGroup;
    emit widgetsGroupChanged();
}

QQuickItem *QQuickDockContainer::dockIndicatorItem() const
{
    Q_D(const QQuickDockContainer);
    return d->dockIndicatorItem;
}
void QQuickDockContainer::setDockIndicatorItem(QQuickItem *item)
{
    Q_D(QQuickDockContainer);

    if (d->dockIndicatorItem == item)
        return;

    addItem(item);
    item->setVisible(false);
    item->setZ(d->zIndicatorItem);
    d->dockIndicatorItem = item;
    emit dockIndicatorItemChanged();
}

QQuickDockContainerAttached *QQuickDockContainer::qmlAttachedProperties(QObject *object)
{
    return new QQuickDockContainerAttached(object);
}

QQuickDockPanel *QQuickDockContainer::activeWidget() const
{
    Q_D(const QQuickDockContainer);
    return d->activePanel;
}

QQuickDockContainerAttached::QQuickDockContainerAttached(QObject *parent)
    : QObject(*(new QQuickDockContainerAttachedPrivate), parent)
{
}

Qt::Edge QQuickDockContainerAttached::edge() const
{
    Q_D(const QQuickDockContainerAttached);
    return d->edge;
}

void QQuickDockContainerAttached::setEdge(Qt::Edge newEdge)
{
    Q_D(QQuickDockContainerAttached);
    d->isFloating = false;
    if (d->edge == newEdge)
        return;
    d->edge = newEdge;
    emit edgeChanged();
}

QQuickDockContainer::PopupType QQuickDockContainer::popupType() const
{
    Q_D(const QQuickDockContainer);
    return d->popupType;
}

void QQuickDockContainer::setPopupType(PopupType newPopupType)
{
    Q_D(QQuickDockContainer);

    if (d->popupType == newPopupType)
        return;
    d->popupType = newPopupType;
    emit popupTypeChanged();
}

bool QQuickDockContainerAttached::isFloating() const
{
    Q_D(const QQuickDockContainerAttached);
    return d->isFloating;
}

void QQuickDockContainerAttached::setIsFloating(bool newIsFloating)
{
    Q_D(QQuickDockContainerAttached);
    if (d->isFloating == newIsFloating)
        return;
    d->isFloating = newIsFloating;
    emit isFloatingChanged();
}

QT_END_NAMESPACE

#include "moc_qquickdockcontainer_p.cpp"
