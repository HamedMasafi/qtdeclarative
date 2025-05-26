// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdocktabbar_p.h"
#include "qquickdocktabbar_p_p.h"
#include "qquickdockpanel_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockTabBar
    \inherits Control
    \instantiates QQuickDockTabBar
    \inqmlmodule QtQuick.Templates
    \internal
    \brief A tab bar control designed for docking panels.

 DockTabBar provides a tab-like interface to switch between multiple dock
 widgets. It is used internally by \l DockArea and \l DockPanelGroup to
 display and manage tabs corresponding to docked panels.

 DockTabBar behaves similarly to the standard QML TabBar but is customized
 for the docking system, handling dock widget specific interactions.

 \section2 Properties

 \qmlproperty variant DockTabBar::dockPanels
 The list of dock widgets associated with the tabs.

 \qmlproperty enumeration DockTabBar::position
 The position of the tab bar, defined by \c Qt::Edge. Typical values are
 \c Qt.TopEdge, \c Qt.BottomEdge, \c Qt.LeftEdge, or \c Qt.RightEdge.

 \qmlproperty int DockTabBar::count
 The number of tabs currently in the tab bar.

 \qmlproperty int DockTabBar::currentIndex
 The index of the currently selected tab.

 \sa DockArea, DockPanelGroup, DockPanel
*/

QQuickDockTabBar::QQuickDockTabBar(QQuickItem *parent)
    : QQuickControl(*(new QQuickDockTabBarPrivate), parent)
{
}

bool QQuickDockTabBar::containsPanel(QQuickDockPanel *panel) const
{

    Q_D(const QQuickDockTabBar);
    return d->widgets.contains(panel);
}

void QQuickDockTabBar::addDockPanel(QQuickDockPanel *widget)
{
    Q_D(QQuickDockTabBar);
    d->widgets << widget;

    setCurrentIndex(d->widgets.size() - 1);
    Q_EMIT dockPanelsChanged();
    Q_EMIT countChanged();
}

void QQuickDockTabBar::removeDockPanel(QQuickDockPanel *widget)
{
    Q_D(QQuickDockTabBar);
    d->widgets.removeOne(widget);

    setCurrentIndex(std::min(currentIndex(), count() - 1));
    Q_EMIT dockPanelsChanged();
    Q_EMIT countChanged();
}

QList<QQuickDockPanel *> QQuickDockTabBar::panels() const
{
    Q_D(const QQuickDockTabBar);
    return d->widgets;
}

QVariant QQuickDockTabBar::dockPanels() const
{
    Q_D(const QQuickDockTabBar);
    QVariantList list;
    for (auto &w : d->widgets)
        list << QVariant::fromValue(w);
    return list;
}

Qt::Edge QQuickDockTabBar::position() const
{
    Q_D(const QQuickDockTabBar);
    return d->position;
}

void QQuickDockTabBar::setPosition(Qt::Edge newPosition)
{
    Q_D(QQuickDockTabBar);
    if (d->position == newPosition)
        return;
    d->position = newPosition;
    Q_EMIT positionChanged();
}

int QQuickDockTabBar::count() const
{
    Q_D(const QQuickDockTabBar);
    return d->widgets.size();
}

QQuickDockPanel *QQuickDockTabBar::currentDockPanel() const
{
    Q_D(const QQuickDockTabBar);
    if (d->currentIndex == -1)
        return nullptr;

    return d->widgets.at(d->currentIndex);
}

void QQuickDockTabBar::setCurrentDockPanel(QQuickDockPanel *widget)
{
    Q_D(const QQuickDockTabBar);
    setCurrentIndex(d->widgets.indexOf(widget));
}

int QQuickDockTabBar::currentIndex() const
{
    Q_D(const QQuickDockTabBar);
    return d->currentIndex;
}

void QQuickDockTabBar::setCurrentIndex(int newCurrentIndex)
{
    Q_D(QQuickDockTabBar);
    // if (d->currentIndex == newCurrentIndex)
    // return;
    d->currentIndex = newCurrentIndex;
    emit currentIndexChanged();
}

qreal QQuickDockTabBar::implicitSize() const
{

    Q_D(const QQuickDockTabBar);
    return d->implicitSize;
}

void QQuickDockTabBar::setInplicitSize(qreal newValue)
{
    Q_D(QQuickDockTabBar);
    if (newValue == d->implicitSize)
        return;
    d->implicitSize = newValue;
    emit implicitSizeChanged();
}

#include "moc_qquickdocktabbar_p.cpp"

QT_END_NAMESPACE
