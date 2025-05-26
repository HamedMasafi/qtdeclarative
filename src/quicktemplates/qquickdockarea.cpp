// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdockarea_p.h"

#include "qquickdockcontainer_p.h"
#include "qquickdockarea_p_p.h"
#include "qquickdockpanel_p.h"
#include "qquickdocksidecontainer_p.h"
#include "qquickdocktabbar_p.h"
#include "qquickdockpanel_p_p.h"
#include "qquickcontentitem_p.h"

#include <private/qquickpopup_p.h>

#include <QCursor>
#include <QDebug>
#include <QPainter>

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockArea
    \inherits Control
    \inqmlmodule QtQuick.Controls
    \brief Represents a container for dock widgets on one edge of a dock layout.

 DockArea is an internal part of the docking system used by \l DockContainer to manage
 dock widgets along one of its four sides: top, left, right, or bottom.

 Each DockArea contains a list of \l DockPanel elements. When at least one dock widget
 is added to the area, a \l DockTabBar is shown to allow the user to switch between them.
 Only one dock widget is visible at a time; others remain hidden unless selected.

 By default, applications do not need to interact directly with DockArea. However, it can
 be accessed via the \l DockContainer's \c topArea, \c leftArea, \c rightArea, and
 \c bottomArea properties for advanced customization.

 \section2 Example (Custom Tab Position)

 \qml
 DockContainer {
     topArea.tabPosition: Qt.TopEdge
 }
 \endqml
*/

/*!
    \qmlproperty int QtQuick.Controls::DockArea::usedSize

 This property holds the currently used size of the dock area,
 which may change as dock widgets are shown or hidden.
*/

/*!
    \qmlproperty DockSideContainer QtQuick.Controls::DockArea::sideContainer

 This property holds the side container that contains this dock area.
 Normally managed internally by \l DockContainer.
*/

/*!
    \qmlproperty Popup QtQuick.Controls::DockArea::popup

 This property holds the popup used to display the dock widget content
 when the dock area is in floating or minimized state.
*/

/*!
    \qmlproperty int QtQuick.Controls::DockArea::count

 This property holds the number of dock widgets currently contained
 in the dock area.
*/

/*!
    \qmlproperty enumeration QtQuick.Controls::DockArea::tabPosition

 This property determines the edge on which the dock tab bar appears.
 It can be one of \c Qt.TopEdge, \c Qt.BottomEdge, \c Qt.LeftEdge,
 or \c Qt.RightEdge.
*/

/*!
    \qmlproperty DockTabBar QtQuick.Controls::DockArea::tabBar

 This property holds the tab bar used to switch between dock widgets
 in the area. It can be replaced to provide a custom tab bar.
*/

/*!
    \qmlproperty Item QtQuick.Controls::DockArea::resizeHandle

 This property holds the item used as a resize handle for adjusting
 the size of the dock area.
*/

QQuickDockAreaPrivate::~QQuickDockAreaPrivate()
{
    // for (auto &dw : dockPanels)
    // removeDockPanel(dw);
}

void QQuickDockAreaPrivate::itemParentChanged(QQuickItem *item, QQuickItem *parent)
{
    Q_UNUSED(parent)
    if (!inDestructor) {
        qDebug() << Q_FUNC_INFO << item << "removed from area";
        removeDockPanel(qobject_cast<QQuickDockPanel *>(item));
        popup->close();
    }
}

void QQuickDockAreaPrivate::addDockPanel(QQuickDockPanel *panel)
{
    Q_Q(QQuickDockArea);
    qDebug() << "Adding panel to area" << panel << "to" << q;
    openGuard = true;

    auto widgetPrivate = QQuickDockPanelPrivate::get(panel);

    if (widgetPrivate->inDestructor)
        return;

    QObjectPrivate::connect(panel, &QQuickDockPanel::visibilityChanged, this,
                            &QQuickDockAreaPrivate::panelVisibilityChanged);

    qDebug() << "Dock widget" << panel << "added to" << this;
    // widgetPrivate->addItemChangeListener(this, ChangeType::Parent);
    // widgetPrivate->detachFromOldParent();
    widgetPrivate->visibility = QQuickDockPanel::Visibility::Unpinned;
    widgetPrivate->edge = tabPosition;
    emit panel->visibilityChanged();

    panel->setVisible(true);

    dockPanels << panel;
    sideContainer->addItem(panel);
    tabBar->addDockPanel(panel);

    // widgetPrivate->setState(QQuickDockPanel::Visibility::Unpinned);

    openGuard = false;
    emit q->dockPanelsChanged();
    emit q->countChanged();
}

void QQuickDockAreaPrivate::removeDockPanel(QQuickDockPanel *panel)
{
    Q_Q(QQuickDockArea);
    dockPanels.removeOne(panel);
    tabBar->removeDockPanel(panel);
    panel->setParent(nullptr);
    // sideContainer->removeItem(widget);
    // QQuickDockPanelPrivate::get(panel)->removeItemChangeListener(this, ChangeType::Parent);

    QObjectPrivate::disconnect(panel, &QQuickDockPanel::visibilityChanged, this,
                               &QQuickDockAreaPrivate::panelVisibilityChanged);
    emit q->dockPanelsChanged();
    emit q->countChanged();
}

void QQuickDockAreaPrivate::updatePolish()
{
    layout();
}

void QQuickDockAreaPrivate::layout()
{
    Q_Q(QQuickDockArea);

    switch (tabPosition) {
    case Qt::TopEdge:
        tabBar->setHeight(40);
        tabBar->setWidth(q->width());
        popup->setX(0);
        popup->setY(q->height());
        popup->setHeight(sideContainer->size());
        popup->setWidth(q->width());
        break;
    case Qt::LeftEdge:
        tabBar->setWidth(40);
        tabBar->setHeight(q->height());
        popup->setX(q->width());
        popup->setWidth(sideContainer->size());
        popup->setHeight(q->height());
        break;
    case Qt::RightEdge:
        tabBar->setWidth(40);
        tabBar->setHeight(q->height());
        popup->setY(0);
        popup->setX(-sideContainer->size()); // p.x() - 300);
        popup->setWidth(sideContainer->size());
        popup->setHeight(q->height());
        break;
    case Qt::BottomEdge:
        tabBar->setHeight(40);
        tabBar->setWidth(q->width());
        popup->setX(0);
        popup->setY(-sideContainer->size());
        popup->setHeight(sideContainer->size());
        popup->setWidth(q->width());
        break;
    }

    // sideContainer->setHeight(popup->height());
    // sideContainer->setWidth(popup->width());
}

void QQuickDockAreaPrivate::updateResizeEdge()
{
    if (Q_LIKELY(tabBar))
        tabBar->setPosition(tabPosition);

    if (Q_LIKELY(sideContainer))
        switch (tabPosition) {
        case Qt::TopEdge:
            sideContainer->setResizeEdge(Qt::BottomEdge);
            break;
        case Qt::LeftEdge:
            sideContainer->setResizeEdge(Qt::RightEdge);
            break;
        case Qt::RightEdge:
            sideContainer->setResizeEdge(Qt::LeftEdge);
            break;
        case Qt::BottomEdge:
            sideContainer->setResizeEdge(Qt::TopEdge);
            break;
        }
}

QQuickItem *QQuickDockAreaPrivate::getContentItem()
{
    Q_Q(QQuickDockArea);
    if (QQuickItem *item = QQuickControlPrivate::getContentItem())
        return item;

    return new QQuickContentItem(q);
}

void QQuickDockAreaPrivate::tabBarCurrentIndexChanged()
{
    Q_Q(QQuickDockArea);

    if (Q_UNLIKELY(!q->isComponentComplete()) && !openGuard)
        return;

    if (tabBar->currentIndex() == -1) {
        popup->close();
        sideContainer->setCurrentIndex(-1);
        return;
    }

    sideContainer->setCurrentIndex(tabBar->currentIndex());
    sideContainer->setWidth(popup->width());
    sideContainer->setHeight(popup->height());
    popup->open();
}

void QQuickDockAreaPrivate::popupClosed()
{
    tabBar->setCurrentIndex(-1);
}

void QQuickDockAreaPrivate::sideContainerSizeChanged()
{
    layout();
}

void QQuickDockAreaPrivate::itemRemoved(int index, QQuickItem *item)
{
    Q_UNUSED(index)
    removeDockPanel(qobject_cast<QQuickDockPanel *>(item));
}

void QQuickDockAreaPrivate::panelVisibilityChanged()
{
    Q_Q(QQuickDockArea);
    auto panel = qobject_cast<QQuickDockPanel *>(q->sender());
    if (!panel)
        return;
    if (panel->visibility() != QQuickDockPanel::Unpinned)
        removeDockPanel(panel);
}

void QQuickDockAreaPrivate::sideContainerPanelAdded(QQuickDockPanel *panel)
{
    tabBar->addDockPanel(panel);
}

void QQuickDockAreaPrivate::sideContainerPanelRemoved(QQuickDockPanel *panel)
{
    tabBar->removeDockPanel(panel);
}

QQuickDockArea::QQuickDockArea(QQuickItem *parent)
    : QQuickControl(*(new QQuickDockAreaPrivate), parent)
{
    // setAcceptHoverEvents(true);
    // setAcceptedMouseButtons(Qt::LeftButton);
}

QQuickDockArea::~QQuickDockArea()
{
    Q_D(QQuickDockArea);
    d->popup->close();
    d->popup->setParentItem(nullptr);
}

void QQuickDockArea::componentComplete()
{
    QQuickControl::componentComplete();

    Q_D(QQuickDockArea);
    d->updateResizeEdge();
    d->tabBar->setCurrentIndex(-1);
    QObjectPrivate::connect(d->tabBar, &QQuickDockTabBar::currentIndexChanged, d,
                            &QQuickDockAreaPrivate::tabBarCurrentIndexChanged);
}

int QQuickDockArea::usedSize() const
{
    // Q_D(const QQuickDockArea);
    // if (!d->dockPanels.size())
    //     return 0;

    // switch (d->tabPosition) {
    // case Qt::TopEdge:
    // case Qt::BottomEdge:
    //     return d->tabBar->height();
    // case Qt::LeftEdge:
    // case Qt::RightEdge:
    //     return d->tabBar->width();
    // }
    return 0; // d->contentModel->count() ? d->tabBar->height() : 0;
}

void QQuickDockArea::addDockPanel(QQuickDockPanel *panel)
{
    Q_D(QQuickDockArea);

    d->addDockPanel(panel);

    Q_EMIT dockPanelsChanged();
}

void QQuickDockArea::removeDockPanel(QQuickDockPanel *panel)
{
    Q_D(QQuickDockArea);
    d->removeDockPanel(panel);
}

// void QQuickDockArea::itemAdded(int index, QQuickItem *item)
// {
//     Q_EMIT usedSizeChanged();
//     QQuickDockGroup::itemAdded(index, item);
// }

// void QQuickDockArea::itemRemoved(int index, QQuickItem *item)
// {
//     Q_EMIT usedSizeChanged();
//     QQuickDockGroup::itemRemoved(index, item);
// }

QQuickDockSideContainer *QQuickDockArea::sideContainer() const
{
    Q_D(const QQuickDockArea);
    return d->sideContainer;
}

void QQuickDockArea::setSideContainer(QQuickDockSideContainer *newSideContainer)
{
    Q_D(QQuickDockArea);

    if (d->sideContainer == newSideContainer)
        return;

    if (d->popup)
        d->popup->setContentItem(newSideContainer);

    QObjectPrivate::connect(newSideContainer, &QQuickDockSideContainer::sizeChanged, d,
                            &QQuickDockAreaPrivate::sideContainerSizeChanged);
    // QObjectPrivate::connect(newSideContainer, &QQuickDockSideContainer::itemRemoved, d, &QQuickDockAreaPrivate::itemRemoved);
    QObjectPrivate::connect(newSideContainer, &QQuickDockSideContainer::panelAdded, d,
                            &QQuickDockAreaPrivate::sideContainerPanelAdded);
    QObjectPrivate::connect(newSideContainer, &QQuickDockSideContainer::panelRemoved, d,
                            &QQuickDockAreaPrivate::sideContainerPanelRemoved);

    if (d->sideContainer) {
        QObjectPrivate::disconnect(d->sideContainer, &QQuickDockSideContainer::sizeChanged, d,
                                   &QQuickDockAreaPrivate::sideContainerSizeChanged);
        // QObjectPrivate::disconnect(newSideContainer, &QQuickDockSideContainer::itemRemoved, d, &QQuickDockAreaPrivate::itemRemoved);
        QObjectPrivate::disconnect(newSideContainer, &QQuickDockSideContainer::panelAdded, d,
                                   &QQuickDockAreaPrivate::sideContainerPanelAdded);
        QObjectPrivate::disconnect(newSideContainer, &QQuickDockSideContainer::panelRemoved, d,
                                   &QQuickDockAreaPrivate::sideContainerPanelRemoved);
    }

    d->sideContainer = newSideContainer;

    emit sideContainerChanged();
}

QQuickPopup *QQuickDockArea::popup() const
{
    Q_D(const QQuickDockArea);
    return d->popup;
}

void QQuickDockArea::setPopup(QQuickPopup *newPopup)
{
    Q_D(QQuickDockArea);
    if (d->popup == newPopup)
        return;
    d->popup = newPopup;

    if (d->sideContainer)
        newPopup->setContentItem(d->sideContainer);
    QObjectPrivate::connect(newPopup, &QQuickPopup::closed, d, &QQuickDockAreaPrivate::popupClosed);
    emit popupChanged();
}

int QQuickDockArea::count() const
{
    Q_D(const QQuickDockArea);
    return d->tabBar->count();
}

Qt::Edge QQuickDockArea::tabPosition() const
{
    Q_D(const QQuickDockArea);
    return d->tabPosition;
}

void QQuickDockArea::setTabPosition(Qt::Edge newTabPosition)
{
    Q_D(QQuickDockArea);

    if (d->tabPosition == newTabPosition)
        return;
    d->tabPosition = newTabPosition;
    d->updateResizeEdge();

    if (d->tabBar)
        d->tabBar->setPosition(d->tabPosition);
    emit tabPositionChanged();
}

QList<QQuickDockPanel *> QQuickDockArea::dockPanels() const
{
    Q_D(const QQuickDockArea);
    return d->tabBar->panels();
}

QQuickDockTabBar *QQuickDockArea::tabBar() const
{
    Q_D(const QQuickDockArea);
    return d->tabBar;
}

void QQuickDockArea::setTabBar(QQuickDockTabBar *newTabBar)
{
    Q_D(QQuickDockArea);
    if (d->tabBar == newTabBar)
        return;
    d->tabBar = newTabBar;
    newTabBar->setPosition(d->tabPosition);
    newTabBar->setParentItem(this); //contentItem());

    Q_EMIT tabBarChanged();
}

void QQuickDockArea::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickControl::geometryChange(oldGeometry, newGeometry);
    Q_D(QQuickDockArea);
    d->layout();
}

QQuickItem *QQuickDockArea::resizeHandle() const
{
    Q_D(const QQuickDockArea);
    return d->resizeHandle;
}

void QQuickDockArea::setResizeHandle(QQuickItem *newResizeHandle)
{
    Q_D(QQuickDockArea);

    if (d->resizeHandle == newResizeHandle)
        return;
    d->resizeHandle = newResizeHandle;
    emit resizeHandleChanged();
}

void QQuickDockArea::open()
{
    Q_D(QQuickDockArea);
    d->popup->close();
}

void QQuickDockArea::close()
{
    Q_D(QQuickDockArea);
    d->popup->close();
}

QT_END_NAMESPACE

#include "moc_qquickdockarea_p.cpp"
