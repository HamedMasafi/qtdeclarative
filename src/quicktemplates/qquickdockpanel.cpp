// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdockpanel_p.h"
#include "qquickdockpanel_p_p.h"
#include "qquickdockwindow_p.h"
#include "qquickdockarea_p.h"
#include "qquickdockcontainer_p.h"
#include "qquickdockgroup_p.h"
#include "qquickdockgroup_p_p.h"
#include "qquickdockcontainer_p_p.h"

#include <QDebug>
#include <QPainter>
#include <QQuickWindow>
#include <QtCore/QCoreApplication>
#include <QScopeGuard>
#include <QGuiApplication>

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockPanel
    \inherits Item
    \inqmlmodule QtQuick.Controls
    \brief A dockable panel with a title, icon, and content.

 DockPanel provides a panel that can be docked inside a \l DockArea or grouped with
 other dock widgets inside a \l DockPanelGroup. It is similar in concept to
 \c QDockWidget from Qt Widgets.

 A DockPanel contains a title, optional icon, and arbitrary QML content. It supports
 docking, undocking, grouping, and activation management.

 \note Changing the \c visible property does not affect whether the panel is considered
 "open" in terms of docking logic. For instance, a dock widget grouped with others may
 be open but not visible if it's not the active tab.

 \section2 Example Usage

 \qml
 DockPanel {
     id: outputPanel
     title: "Output"
     icon.name: "output-icon"

     Rectangle {
         color: "lightgray"
         anchors.fill: parent
     }
 }
 \endqml
*/

/*!
    \qmlproperty string QtQuick.Controls::DockPanel::title

 This property holds the title of the dock widget.
*/

/*!
    \qmlproperty Item QtQuick.Controls::DockPanel::titleBar

 This property holds a custom item to be used as the title bar.
 If not set, a default title bar is used.
*/

/*!
    \qmlproperty icon QtQuick.Controls::DockPanel::icon

 This property holds the icon shown on the dock widget's title bar.
*/

/*!
    \qmlproperty bool QtQuick.Controls::DockPanel::isActive

 This property is \c true if the dock widget is the currently active one in its group,
 otherwise \c false.
*/

/*!
    \qmlproperty int QtQuick.Controls::DockPanel::resizeEdgeSize

 This property controls how far from the edge the mouse can be for resizing the panel.
*/

/*!
    \qmlmethod void QtQuick.Controls::DockPanel::open()

 Opens the dock widget. This may make it visible or activate it depending on context.
 A dock widget can be open without being visible (for example, if it's not the active
 tab in a group).
*/

/*!
    \qmlmethod void QtQuick.Controls::DockPanel::close()

 Closes the dock widget, hiding it and marking it as closed in docking logic.
*/

QQuickDockPanelPrivate::QQuickDockPanelPrivate()
{
    isWayland = QGuiApplication::platformName().contains(QStringLiteral("wayland"),
                                                         Qt::CaseInsensitive);
}

void QQuickDockPanelPrivate::itemGeometryChanged(QQuickItem *item, QQuickGeometryChange change,
                                                 const QRectF &diff)
{
    QQuickControlPrivate::itemGeometryChanged(item, change, diff);

    relayout();
}

void QQuickDockPanelPrivate::itemVisibilityChanged(QQuickItem *item)
{
    QQuickControlPrivate::itemVisibilityChanged(item);
    // QScopedValueRollback signalGuard(emittingImplicitSizeChangedSignals, true);
    if (item == titleBarItem) {
        // emit q->implicitHeaderWidthChanged();
        // emit q->implicitHeaderHeightChanged();
        relayout();
    }
}

void QQuickDockPanelPrivate::resizeToRect(const QPointF &pt)
{
    if (rectBeforeResize.contains(pt))
        return;

    qreal relativeX = (pt.x() - rectBeforeResize.left()) / rectBeforeResize.width();
    relativeX = qBound(0.0, relativeX, 1.0); // clamp to [0, 1]

    QPointF newTopLeft;
    newTopLeft.setX(pt.x() - relativeX * rectBeforeResize.width());
    newTopLeft.setY(pt.y() - titleBarItem->height() / 2.0); // center pt vertically in the top area

    rectBeforeResize.moveTopLeft(newTopLeft);
}

void QQuickDockPanelPrivate::setVisibility(QQuickDockPanel::Visibility v)
{
    Q_Q(QQuickDockPanel);

    if (v == this->visibility)
        return;

    detachFromOldParent();

    switch (v) {
    case QQuickDockPanel::Closed:
        isClosed = true;
        if (dockWindow)
            dockWindow->close();
        else
            q->setVisible(false);
        emit q->closed();
        break;
    case QQuickDockPanel::Unpinned:
        QQuickDockContainerPrivate::get(dockContainer)->addWidget(q, edge);
        break;
    case QQuickDockPanel::Pinned:
        if (!group && homeGroup)
            group = homeGroup;

        if (group) {
            group->addItem(q);
            //            QQuickDockGroupPrivate::get(group)->addDockPanel(q);
        } else {
            qWarning("DockPanel: cannot pin without a homeGroup");
            return;
        }
        // break;
        // if (group)
        // group->addItem(q);
        break;
    case QQuickDockPanel::Detached:
        detach();
        if (dockWindow) {
            if (!isWayland) {
                dockWindow->setPosition(rectBeforeResize.topLeft().toPoint());
                dockWindow->resize(rectBeforeResize.size().toSize());
            }
        } else {
            q->setPosition(rectBeforeResize.topLeft());
            q->setSize(rectBeforeResize.size());
        }
        break;
    }

    this->visibility = v;
    emit q->visibilityChanged();
}

void QQuickDockPanelPrivate::setEdge(Qt::Edge newEdge)
{
    Q_Q(QQuickDockPanel);
    if (q->isComponentComplete() && visibility == QQuickDockPanel::Visibility::Unpinned) {
        auto containerPrivate = QQuickDockContainerPrivate::get(dockContainer);
        containerPrivate->area(edge)->removeDockPanel(q);
        containerPrivate->addWidget(q, newEdge);
    }

    this->edge = newEdge;
    emit q->edgeChanged();
}

void QQuickDockPanelPrivate::detach()
{
    Q_Q(QQuickDockPanel);

    Q_ASSERT(dockContainer);

    if (visibility == QQuickDockPanel::Visibility::Detached)
        return;

    if (QQuickDockContainerPrivate::get(dockContainer)->usePopupWindow()) {
        auto wpos = q->mapToGlobal(QPointF(0, 0)).toPoint();

        dockWindow->setDockPanel(q);
        dockWindow->setTitle(title);

        resizeToRect(wpos);
        dockWindow->setPosition(QCursor::pos()); // rectBeforeResize.topLeft().toPoint());
        dockWindow->resize(rectBeforeResize.size().toSize());
        dockWindow->show();
        // isDetached = true;
    } else {
        auto wpos = q->mapToItem(dockContainer, QPointF(0, 0)).toPoint();

        resizeToRect(wpos);

        q->setParentItem(dockContainer->contentItem());
        q->setX(floatingGeometry.x());
        q->setY(floatingGeometry.y());
        q->setSize(floatingGeometry.size());
        q->setVisible(true);
        q->setZ(QQuickDockContainerPrivate::zFloatingWidgetStart);
        // isDetached = false;
    }

    detachFromOldParent();
    if (visibility == QQuickDockPanel::Visibility::Pinned)
        QQuickDockGroupPrivate::get(group)->removeDockPanel(q);

    visibility = QQuickDockPanel::Visibility::Detached;
    emit q->detached();
    emit q->visibilityChanged();
}

void QQuickDockPanelPrivate::resizeContent()
{
    relayout();
}

void QQuickDockPanelPrivate::setInitState()
{
    Q_Q(QQuickDockPanel);
    qDebug() << Q_FUNC_INFO << title << visibility;
    switch (visibility) {
    case QQuickDockPanel::Closed:
        setVisible(false);
        break;

    case QQuickDockPanel::Unpinned:
        QQuickDockContainerPrivate::get(dockContainer)->area(edge)->addDockPanel(q);
        break;
    case QQuickDockPanel::Pinned:
    case QQuickDockPanel::Detached:
        break;
    }
}
void QQuickDockPanelPrivate::unpin()
{
    Q_Q(QQuickDockPanel);
    detachFromOldParent();
    QQuickDockContainerPrivate::get(dockContainer)->area(edge)->addDockPanel(q);
    visibility = QQuickDockPanel::Unpinned;
    emit q->visibilityChanged();
}

void QQuickDockPanelPrivate::pinTo(QQuickDockGroup *g)
{
    Q_Q(QQuickDockPanel);

    if (g == homeGroup)
        return;
    if (visibility == QQuickDockPanel::Visibility::Pinned && g == group)
        return;

    detachFromOldParent();

    if (group) {
        auto oldGroupPrivate = QQuickDockGroupPrivate::get(group);
        oldGroupPrivate->removeDockPanel(q);
        // g->addItem(q);
    }
    // if (g)
    this->group = g;

    // QQuickDockGroupPrivate::get(g)->addDockPanel(q);
    group->addItem(q);

    visibility = QQuickDockPanel::Visibility::Pinned;
    emit q->visibilityChanged();
}

void QQuickDockPanelPrivate::relayout()
{
    Q_Q(QQuickDockPanel);
    const qreal hh = titleBarItem ? titleBarItem->height() : 0;
    const qreal hsp = hh > 0 ? spacing : 0;

    if (titleBarItem) {
        titleBarItem->setY(resizeEdgeSize);
        titleBarItem->setX(resizeEdgeSize);
        titleBarItem->setWidth(q->width() - 2 * resizeEdgeSize);
    }

    if (contentItem) {
        contentItem->setY(q->topPadding() + hh + hsp);
        contentItem->setX(q->leftPadding());
        contentItem->setWidth(q->availableWidth());
        contentItem->setHeight(q->availableHeight() - hh - hsp);
    }
}

/*
 * Detach from old parent; It won't detach from QQuickDockGroup, instead just remove from
 * the group and stay in group's control until attaching to same or other group
 */
void QQuickDockPanelPrivate::detachFromOldParent()
{
    Q_Q(QQuickDockPanel);

    switch (visibility) {
    case QQuickDockPanel::Closed:
        break;
    case QQuickDockPanel::Unpinned:
        // if (dockContainer)
        // QQuickDockContainerPrivate::get(dockContainer)->area(edge)->removeDockPanel(q);
        break;
    case QQuickDockPanel::Pinned:
        // if (group)
        // QQuickDockGroupPrivate::get(group)->removeDockPanel(q);
        // group.remo
        break;
    case QQuickDockPanel::Detached:
        if (dockWindow)
            dockWindow->hide();
        else
            q->setVisible(false);
        break;
    }
}
void QQuickDockPanelPrivate::headerMoveStarted(QMouseEvent *event)
{
    Q_Q(QQuickDockPanel);

    // setParentItem(d->dockContainer);

    auto containerPrivate = QQuickDockContainerPrivate::get(dockContainer);
    if (containerPrivate->usePopupWindow()) {

        // dockWindow->setPosition(event->globalPosition().x() - 3, event->globalPosition().y() -
        // 4);
        if (!isWayland) {
            moveClickPos = event->globalPosition();
            moveStartPos = q->mapToGlobal(0, 0);
            event->accept();
        }

        createDockWindow();
        detach();

        if (!isWayland)
            dockWindow->startSystemMove();
    } else {
        moveClickPos = event->scenePosition();
        moveStartPos = q->mapToItem(dockContainer->contentItem(), 0, 0);
        auto size = q->size();
        detach();
        q->setPosition(moveStartPos);
        q->setSize(size);

        qDebug() << "Move started" << moveStartPos << size;
        event->accept();
    }
    containerPrivate->setActiveWidget(q);
    Q_EMIT q->beginMove();
}

void QQuickDockPanelPrivate::headerMoving(QMouseEvent *event)
{
    Q_Q(QQuickDockPanel);
    if (dockWindow) {
        auto pt = event->globalPosition();
        // dockWindow->setPosition(((event->globalPos() - moveClickPos) + moveStartPos).toPoint());

        // qDebug() << "moving" << ((event->globalPos() - moveClickPos) + moveStartPos).toPoint();
        Q_EMIT q->moving(pt);
    } else {
        q->setPosition((event->scenePosition() - moveClickPos) + moveStartPos);
        Q_EMIT q->moving(event->globalPosition());
    }
}

void QQuickDockPanelPrivate::headerMoveEnded()
{
    qDebug() << Q_FUNC_INFO;
    Q_Q(QQuickDockPanel);

    if (dockWindow) { }
    Q_EMIT q->moved();
}

void QQuickDockPanelPrivate::headerDoubleClicked()
{
    if (visibility == QQuickDockPanel::Visibility::Pinned)
        detach();
    else
        attach();
}

void QQuickDockPanelPrivate::setActive(bool active)
{
    if (isActive == active)
        return;
    Q_Q(QQuickDockPanel);
    isActive = active;
    Q_EMIT q->isActiveChanged(isActive);
}

void QQuickDockPanelPrivate::attach()
{
    Q_Q(QQuickDockPanel);

    if (visibility == QQuickDockPanel::Visibility::Pinned)
        return;

    if (!group)
        return;

    if (dockWindow)
        dockWindow->hide();

    group->addItem(q);
}

void QQuickDockPanelPrivate::open()
{
    Q_Q(QQuickDockPanel);

    if (visibility != QQuickDockPanel::Visibility::Closed)
        return;

    visibility = oldVisibility;
    /*    if (state == QQuickDockPanel::Visibility::Detached) {
        if (dockWindow)
            dockWindow->show();
        else
            q->setVisible(true);
    }
*/
    qDebug() << "open" << oldVisibility;
    switch (oldVisibility) {
    case QQuickDockPanel::Closed:
        break;
    case QQuickDockPanel::Unpinned:
        QQuickDockContainerPrivate::get(dockContainer)->area(edge)->addDockPanel(q);
        break;
    case QQuickDockPanel::Pinned:
        if (group)
            group->addItem(q);
        else
            qWarning() << "No group";
        break;
    case QQuickDockPanel::Detached:
        if (dockWindow) {
            dockWindow->setDockPanel(q);
            dockWindow->show();
        } else {
            q->setParentItem(QQuickDockContainerPrivate::get(dockContainer)->contentItem);
            q->setVisible(true);
        }
        break;
    }

    emit q->visibilityChanged();
    emit q->isOpenChanged();
}

void QQuickDockPanelPrivate::close()
{
    Q_Q(QQuickDockPanel);
    qDebug() << Q_FUNC_INFO;
    // isOpen = false;

    // detachFromOldParent();
    oldVisibility = visibility;
    visibility = QQuickDockPanel::Closed;

    auto containerPrivate = QQuickDockContainerPrivate::get(dockContainer);
    if (containerPrivate->activePanel == q)
        containerPrivate->setActiveWidget(nullptr);

    q->setParentItem(nullptr);
    if (dockWindow)
        dockWindow->setVisible(false);

    emit q->isOpenChanged();
    emit q->visibilityChanged();
}

QQuickDockPanel::Visibility QQuickDockPanel::visibility() const
{
    Q_D(const QQuickDockPanel);
    return d->visibility;
}

void QQuickDockPanelPrivate::createDockWindow()
{
    Q_Q(QQuickDockPanel);
    if (dockWindow)
        return;
    dockWindow = new QQuickDockWindow;
    dockWindow->setTransientParent(dockContainer->window());
    dockWindow->installEventFilter(q);

    QObject::connect(q, &QQuickDockPanel::titleChanged, dockWindow, &QQuickWindow::setTitle);
}

QQuickDockPanel::QQuickDockPanel(QQuickItem *parent)
    : QQuickControl(*(new QQuickDockPanelPrivate), parent)
{
    // setClip(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFiltersChildMouseEvents(true);
}

QQuickDockPanel::~QQuickDockPanel()
{
    Q_D(QQuickDockPanel);
    setParentItem(nullptr);
    if (d->dockWindow)
        d->dockWindow->deleteLater();
}

void QQuickDockPanel::open()
{
    Q_D(QQuickDockPanel);
    d->open();
}

void QQuickDockPanel::close()
{
    Q_D(QQuickDockPanel);
    d->close();
}

void QQuickDockPanel::restoreSize()
{
    Q_D(QQuickDockPanel);
    setSize(d->originalSize);
}

void QQuickDockPanel::setTitle(QString title)
{
    Q_D(QQuickDockPanel);
    if (d->title == title)
        return;

    d->title = title;
    Q_EMIT titleChanged(d->title);
}

void QQuickDockPanel::setTitleBar(QQuickItem *titleBar)
{
    Q_D(QQuickDockPanel);

    if (d->titleBarItem == titleBar)
        return;

    if (qFuzzyIsNull(titleBar->z()))
        titleBar->setZ(1);
    titleBar->setParentItem(this);
    titleBar->setVisible(true);
    d->titleBarItem = titleBar;
    d->titleBarItem->setAcceptedMouseButtons(Qt::LeftButton);
    d->titleBarItem->setKeepMouseGrab(true);

    if (isComponentComplete())
        d->relayout();
    Q_EMIT titleBarChanged(d->titleBarItem);
}

void QQuickDockPanel::setIsActive(bool isActive)
{
    Q_D(QQuickDockPanel);
    d->setActive(isActive);
}

bool QQuickDockPanel::childMouseEventFilter(QQuickItem *item, QEvent *e)
{
    Q_D(QQuickDockPanel);

    static bool _moveEmitted = false;

    if (item == d->contentItem && e->type() == QEvent::MouseButtonPress) {
        QQuickDockContainerPrivate::get(d->dockContainer)->setActiveWidget(this);
    }
    if (item != d->titleBarItem) {
        return QQuickControl::childMouseEventFilter(item, e);
    }

    auto me = static_cast<QMouseEvent *>(e);

    switch (e->type()) {
    case QEvent::MouseButtonPress:
        _moveEmitted = false;
        me->accept();
        return true;
        break;

    case QEvent::MouseMove: {
        if (_moveEmitted) {
            d->headerMoving(me);
        } else {
            d->headerMoveStarted(me);
            _moveEmitted = true;
        }
        // me->accept();
        break;
    }

    case QEvent::MouseButtonRelease:
        _moveEmitted = false;
        d->headerMoveEnded();
        me->accept();
        break;

    case QEvent::MouseButtonDblClick:
        d->headerDoubleClicked();
        break;

    default:
        break;
    }

    return QQuickControl::childMouseEventFilter(item, e);
}

void QQuickDockPanel::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QQuickDockPanel);
    if (!isComponentComplete())
        return;

    QQuickControl::geometryChange(newGeometry, oldGeometry);

    if (d->visibility == QQuickDockPanel::Visibility::Detached) {
        d->originalSize = newGeometry.size();
    }

    d->relayout();
}

void QQuickDockPanel::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(QQuickDockPanel);

    if (d->visibility != QQuickDockPanel::Visibility::Detached)
        return;

    QCursor cursor = Qt::ArrowCursor;

    if (event->position().x() < d->resizeEdgeSize && event->position().y() < d->resizeEdgeSize)
        cursor = Qt::SizeFDiagCursor;
    else if (event->position().x() >= width() - d->resizeEdgeSize
             && event->position().y() >= height() - d->resizeEdgeSize)
        cursor = Qt::SizeFDiagCursor;
    else if (event->position().x() >= width() - d->resizeEdgeSize
             && event->position().y() < d->resizeEdgeSize)
        cursor = Qt::SizeBDiagCursor;
    else if (event->position().x() < d->resizeEdgeSize
             && event->position().y() >= height() - d->resizeEdgeSize)
        cursor = Qt::SizeBDiagCursor;
    else if (event->position().x() < d->resizeEdgeSize
             || event->position().x() >= width() - d->resizeEdgeSize)
        cursor = Qt::SizeHorCursor;
    else if (event->position().y() < d->resizeEdgeSize
             || event->position().y() >= height() - d->resizeEdgeSize)
        cursor = Qt::SizeVerCursor;

    setCursor(cursor);
}

void QQuickDockPanel::mousePressEvent(QMouseEvent *event)
{
    Q_D(QQuickDockPanel);

    qDebug() << Q_FUNC_INFO << event->scenePosition();
    if (d->visibility != QQuickDockPanel::Visibility::Detached)
        return;
    d->resizeEdge = 0;

    if (event->pos().x() < d->resizeEdgeSize)
        d->resizeEdge |= Qt::LeftEdge;
    if (event->pos().x() >= width() - d->resizeEdgeSize)
        d->resizeEdge |= Qt::RightEdge;
    if (event->pos().y() < d->resizeEdgeSize)
        d->resizeEdge |= Qt::TopEdge;
    if (event->pos().y() >= height() - d->resizeEdgeSize)
        d->resizeEdge |= Qt::BottomEdge;

    if (d->resizeEdge) {
        /*if (d->isDetached && d->dockWindow) {
            d->dockWindow->startSystemResize((Qt::Edge)d->resizeEdge);
            event->ignore();
        } else {
        }*/
        event->accept();
        d->rectBeforeResize = QRectF(position(), size());
        d->resizeMousePos = event->scenePosition();
    } else {
        event->ignore();
    }
    QQuickControl::mousePressEvent(event);
}

bool QQuickDockPanel::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(QQuickDockPanel);
    if (watched == d->dockWindow) {
        if (event->type() == QEvent::Move) {
            // emit moved();
        }
    }

    return QQuickControl::eventFilter(watched, event);
}

void QQuickDockPanel::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QQuickDockPanel);

    qDebug() << Q_FUNC_INFO << event->scenePosition();
    if (d->visibility != QQuickDockPanel::Detached)
        return;

    auto rc = d->rectBeforeResize;

    if (d->resizeEdge & Qt::LeftEdge)
        rc.setLeft(d->rectBeforeResize.left() + event->scenePosition().x() - d->resizeMousePos.x());

    if (d->resizeEdge & Qt::TopEdge)
        rc.setTop(d->rectBeforeResize.top() + event->scenePosition().y() - d->resizeMousePos.y());

    if (d->resizeEdge & Qt::RightEdge)
        rc.setRight(d->rectBeforeResize.right() + event->scenePosition().x()
                    - d->resizeMousePos.x());

    if (d->resizeEdge & Qt::BottomEdge)
        rc.setBottom(d->rectBeforeResize.bottom() + event->scenePosition().y()
                     - d->resizeMousePos.y());

    setPosition(rc.topLeft());
    setSize(rc.size());

    QQuickControl::mouseMoveEvent(event);
}

void QQuickDockPanel::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
}

QQuickDockWindow *QQuickDockPanel::dockWindow() const
{
    Q_D(const QQuickDockPanel);
    return d->dockWindow;
}

QQuickItem *QQuickDockPanel::titleBar() const
{
    Q_D(const QQuickDockPanel);
    return d->titleBarItem;
}

void QQuickDockPanel::componentComplete()
{
    Q_D(QQuickDockPanel);
    QQuickControl::componentComplete();
    d->relayout();
    d->setInitState();
}

bool QQuickDockPanel::isActive() const
{
    Q_D(const QQuickDockPanel);
    return d->isActive;
}

QString QQuickDockPanel::title() const
{
    Q_D(const QQuickDockPanel);
    return d->title;
}

QQuickIcon QQuickDockPanel::icon() const
{
    Q_D(const QQuickDockPanel);
    return d->icon;
}

void QQuickDockPanel::setIcon(const QQuickIcon &newIcon)
{
    Q_D(QQuickDockPanel);

    if (d->icon == newIcon)
        return;
    d->icon = newIcon;
    emit iconChanged();
}

int QQuickDockPanel::resizeEdgeSize() const
{
    Q_D(const QQuickDockPanel);
    return d->resizeEdgeSize;
}

void QQuickDockPanel::setResizeEdgeSize(int newResizeEdgeSize)
{
    Q_D(QQuickDockPanel);

    if (d->resizeEdgeSize == newResizeEdgeSize)
        return;
    d->resizeEdgeSize = newResizeEdgeSize;
    emit resizeEdgeSizeChanged();
}

bool QQuickDockPanel::isOpened() const
{
    Q_D(const QQuickDockPanel);
    return d->visibility != QQuickDockPanel::Visibility::Closed;
}

void QQuickDockPanel::setIsOpened(bool newIsOpen)
{
    Q_D(QQuickDockPanel);

    if (newIsOpen == isOpened())
        return;

    if (newIsOpen)
        d->open();
    else
        d->close();
}

bool QQuickDockPanel::floating() const
{
    Q_D(const QQuickDockPanel);

    return d->visibility == QQuickDockPanel::Detached;
}

bool QQuickDockPanel::pinned() const
{
    Q_D(const QQuickDockPanel);
    return d->visibility == Visibility::Pinned;
}

void QQuickDockPanel::togglePin()
{
    Q_D(QQuickDockPanel);
    d->setVisibility(d->visibility == Pinned ? QQuickDockPanel::Visibility::Unpinned
                                             : QQuickDockPanel::Visibility::Pinned);
}

void QQuickDockPanel::pinTo(QQuickDockGroup *g)
{
    // Q_D(QQuickDockPanel);
    // d->pinTo(g);
    g->addItem(this);
}

void QQuickDockPanel::setVisibility(const Visibility &newVisibility)
{
    Q_D(QQuickDockPanel);

    if (Q_LIKELY(isComponentComplete()))
        d->setVisibility(newVisibility);
    else
        d->visibility = newVisibility;
}

Qt::Edge QQuickDockPanel::edge() const
{
    Q_D(const QQuickDockPanel);

    return d->edge;
}

void QQuickDockPanel::setEdge(Qt::Edge newEdge)
{
    Q_D(QQuickDockPanel);

    d->setEdge(newEdge);
}

QT_END_NAMESPACE

#include "moc_qquickdockpanel_p.cpp"
