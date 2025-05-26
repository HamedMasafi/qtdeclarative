// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qquickdockgroup_p.h"

#include "qquickdockgroup_p_p.h"
#include "qquickdockcontainer_p_p.h"
#include "qquickdocktabbar_p.h"
#include "qquickdockpanel_p_p.h"
#include "qquickcontentitem_p.h"

#include <QCursor>

QT_BEGIN_NAMESPACE

/*!
    \qmltype DockGroup
    \inherits Item
    \instantiates QQuickDockGroup
    \inqmlmodule QtQuick.Templates
    \internal
    \brief A container for managing a tabbed group of dock widgets.

 WidgetGroup is an internal type used to group multiple \l DockPanel
 items together into a single tabbed container. It displays a \l DockTabBar
 that allows the user to switch between the dock widgets in the group.

 When one of the widgets is selected through the tab bar, it becomes the
 active widget, and only the active one is shown. The others remain hidden
 but retain their state.

 This component is typically used by \l DockLayout and \l DockContainer and is
 not intended to be used directly in user-facing QML code unless you are
 customizing the docking system.

 \section2 Properties

 \qmlproperty DockTabBar DockGroup::tabBar
 The tab bar used to switch between the dock widgets.

 \qmlproperty list<DockPanel> DockGroup::dockPanels
 The list of dock widgets currently contained in the group.

 \qmlproperty enumeration DockGroup::tabPosition
 The position of the tab bar. This follows the \c Qt::Edge enumeration,
 typically one of \c Qt.TopEdge, \c Qt.BottomEdge, etc.

 \qmlproperty bool DockGroup::isActive
 Whether this group is currently active. Only one DockGroup is
 active at a time in a given layout.

 \sa DockPanel, DockLayout, DockTabBar
*/

QQuickDockGroupPrivate::~QQuickDockGroupPrivate()
{
    // for (auto &widget : dockPanels) {
    // auto widgetPrivate = QQuickDockPanelPrivate::get(widget);

    // widgetPrivate->removeItemChangeListener(this, ChangeType::Parent);
    // }
}

QQuickDockGroupPrivate *QQuickDockGroupPrivate::get(QQuickDockGroup *area)
{
    return area->d_func();
}

QQuickItem *QQuickDockGroupPrivate::getContentItem()
{
    Q_Q(QQuickDockGroup);
    if (QQuickItem *item = QQuickContainerPrivate::getContentItem())
        return item;

    return new QQuickContentItem(q);
}

void QQuickDockGroupPrivate::itemParentChanged(QQuickItem *item, QQuickItem *parent)
{
    if (!parent) {
        QQuickContainerPrivate::itemParentChanged(item, parent);
        return;
    }

    qDebug() << Q_FUNC_INFO << item << parent;
    if (auto widget = qobject_cast<QQuickDockPanel *>(item); widget && parent != contentItem) {
        qDebug() << "Removing";
        removeDockPanel(widget);
    }
}

void QQuickDockGroupPrivate::updatePolish()
{
    // layout();
}

void QQuickDockGroupPrivate::layout()
{
    Q_Q(QQuickDockGroup);

    if (!Q_UNLIKELY(componentComplete))
        return;
    const int count = contentModel->count();

    if (tabBar->count() != contentModel->count()) {
        qDebug() << "Assert faild; tabBar.count=" << tabBar->count()
                 << "  contentmodel.count=" << contentModel->count();
        auto tabBarPanels = tabBar->panels();
        for (auto i = 0; i < tabBarPanels.count(); ++i)
            qDebug() << "Tabbar " << i << tabBarPanels[i];
        for (int i = 0; i < count; ++i) {
            auto item = itemAt(i);
            if (item == tabBar)
                continue;

            qDebug() << "ContentModel" << i << item;
        }
    }

    if (layingOut)
        return;

    if (count <= 0)
        return;

    layingOut = true;
    for (int i = 0; i < count; ++i) {
        auto item = itemAt(i);
        if (item == tabBar)
            continue;

        item->setPosition(QPointF{ q->leftInset(), q->topInset() });
        item->setWidth(q->availableWidth() - q->leftInset() - q->rightInset());
        item->setHeight(q->availableHeight() - q->topInset() - q->bottomInset());
        item->setVisible(i == currentIndex);
    }
    layingOut = false;
}

void QQuickDockGroupPrivate::currentIndexChanged()
{
    // if (currentIndex >= 0 && currentIndex < contentModel->count()
    //     && Q_UNLIKELY(q->isComponentComplete()))
    //     QQuickDockContainerPrivate::get(dockContainer)
    //             ->setActiveWidget(qobject_cast<QQuickDockPanel *>(itemAt(currentIndex)));

    // // Q_ASSERT(currentIndex >= -1 && currentIndex < dockPanels.size());
    // isActive = true;
    layout();
}

void QQuickDockGroupPrivate::setActive(bool active)
{
    if (active == isActive)
        return;

    isActive = active;
}

void QQuickDockGroupPrivate::checkForCount()
{
    Q_Q(QQuickDockGroup);

    if (contentModel->count()) {
        q->setVisible(true);
        layout();
        return;
    }

    if (detachedDockPanels.size()) {
        q->setVisible(false);
        return;
    }

    q->setVisible(false);
    // q->deleteLater();
}

void QQuickDockGroupPrivate::addDockPanel(QQuickDockPanel *widget)
{
    Q_Q(QQuickDockGroup);

    if (tabBar->containsPanel(widget)) {
        qDebug() << "The widget already belogngs to this group";
        return;
    }

    if (detachedDockPanels.contains(widget)) { }

    auto panelPrivate = QQuickDockPanelPrivate::get(widget);

    // if (panelPrivate->group && panelPrivate->group != q) {
    // QQuickDockGroupPrivate::get(panelPrivate->group)->detachDockPanel(widget);
    // }

    connect(widget, &QQuickDockPanel::isActiveChanged, this,
            &QQuickDockGroupPrivate::widgetActiveChanged, Qt::UniqueConnection);
    connect(widget, &QQuickDockPanel::visibilityChanged, this,
            &QQuickDockGroupPrivate::panelVisibilityChanged, Qt::UniqueConnection);

    checkForCount();
    tabBar->addDockPanel(widget);

    if (q->isComponentComplete()) {
        // panelPrivate->visibility = QQuickDockPanel::Visibility::Pinned;
        panelPrivate->visibility =
                QQuickDockPanel::Visibility::Pinned; // emit widget->visibilityChanged();
    }
    panelPrivate->group = q;

    emit widget->visibilityChanged();

    emit q->dockPanelsChanged();
}

void QQuickDockGroupPrivate::removeDockPanel(QQuickDockPanel *widget)
{
    if (!tabBar->containsPanel(widget)) {
        qDebug() << Q_FUNC_INFO << "Not found ";
        return;
    }
    layingOut = true;

    tabBar->removeDockPanel(widget);

    auto index = contentModel->indexOf(widget, nullptr);
    if (index != -1)
        contentModel->remove(index);

    layingOut = false;
    checkForCount();
    qDebug() << "Item reoved; children size=" << contentModel->count();
}

void QQuickDockGroupPrivate::detachDockPanel(QQuickDockPanel *widget)
{
    removeDockPanel(widget);

    detachedDockPanels.removeOne(widget);

    disconnect(widget, &QQuickDockPanel::isActiveChanged, this,
               &QQuickDockGroupPrivate::widgetActiveChanged);
    disconnect(widget, &QQuickDockPanel::visibilityChanged, this,
               &QQuickDockGroupPrivate::panelVisibilityChanged);

    layingOut = true;
    tabBar->removeDockPanel(widget);
    auto index = contentModel->indexOf(widget, nullptr);
    if (index != -1)
        contentModel->remove(index);
    layingOut = false;
    checkForCount();
}

void QQuickDockGroupPrivate::widgetActiveChanged()
{
    Q_Q(QQuickDockGroup);
    auto w = qobject_cast<QQuickDockPanel *>(q->sender());
    if (!w)
        return;

    if (w->isActive() != isActive) {
        isActive = w->isActive();
        layout();
        Q_EMIT q->isActiveChanged();
    }
}

void QQuickDockGroupPrivate::tabbarCountChanged()
{
    // Q_Q(QQuickDockGroup);
    // q->setVisible(tabBar->count());
}

void QQuickDockGroupPrivate::tabbarCurrentIndexChanged()
{
    currentIndex = tabBar->currentIndex();
    layout();
}

void QQuickDockGroupPrivate::panelVisibilityChanged()
{
    Q_Q(QQuickDockGroup);
    auto panel = qobject_cast<QQuickDockPanel *>(q->sender());

    if (panel && panel->visibility() != QQuickDockPanel::Pinned)
        removeDockPanel(panel);
}

QQuickDockGroup::QQuickDockGroup(QQuickItem *parent)
    : QQuickContainer(*(new QQuickDockGroupPrivate), parent)
{
    Q_D(QQuickDockGroup);

    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    QObjectPrivate::connect(this, &QQuickContainer::currentIndexChanged, d,
                            &QQuickDockGroupPrivate::currentIndexChanged);
}

QQuickDockGroup::~QQuickDockGroup()
{
    Q_D(QQuickDockGroup);
    for (int i = 0; i < d->contentModel->count(); ++i) {
        QQuickItem *item = qobject_cast<QQuickItem *>(d->contentModel->object(i));
        d->removeImplicitSizeListener(item);
    }
}

void QQuickDockGroup::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(QQuickDockGroup);
    QQuickContainer::geometryChange(newGeometry, oldGeometry);
    if (Q_LIKELY(isComponentComplete()))
        d->layout();
}

void QQuickDockGroup::itemAdded(int index, QQuickItem *item)
{
    Q_UNUSED(index)
    Q_D(QQuickDockGroup);

    qDebug() << Q_FUNC_INFO << item;
    auto w = qobject_cast<QQuickDockPanel *>(item);

    if (w) {
        d->addDockPanel(w);
    }
    polish();
}

void QQuickDockGroup::itemRemoved(int index, QQuickItem *item)
{
    Q_UNUSED(index)
    Q_D(QQuickDockGroup);

    if (QQuickItemPrivate::get(item)->isTransparentForPositioner())
        return;

    auto w = qobject_cast<QQuickDockPanel *>(item);

    if (w) {
        d->removeDockPanel(w);
    }
}
bool QQuickDockGroup::isContent(QQuickItem *item) const
{
    Q_D(const QQuickDockGroup);
    if (!qmlContext(item))
        return false;

    if (QQuickItemPrivate::get(item)->isTransparentForPositioner())
        return false;

    return item != d->tabBar;
}

QQuickDockPanel *QQuickDockGroup::activePanel() const
{
    Q_D(const QQuickDockGroup);
    return d->activePanel;
}

QQuickDockTabBar *QQuickDockGroup::tabBar() const
{
    Q_D(const QQuickDockGroup);

    return d->tabBar;
}

void QQuickDockGroup::setTabBar(QQuickDockTabBar *newTabBar)
{
    Q_D(QQuickDockGroup);

    if (d->tabBar == newTabBar)
        return;

    if (d->tabBar) {
        // QObjectPrivate::disconnect(d->tabBar, &QQuickDockTabBar::countChanged, d,
        // &QQuickDockGroupPrivate::tabbarCountChanged);
    }
    QObjectPrivate::connect(newTabBar, &QQuickDockTabBar::currentIndexChanged, d,
                            &QQuickDockGroupPrivate::tabbarCurrentIndexChanged);

    d->tabBar = newTabBar;
    newTabBar->setParentItem(this);
    newTabBar->setVisible(true);
    emit tabBarChanged();
}

void QQuickDockGroup::updatePolish()
{
    QQuickContainer::updatePolish();
    Q_D(QQuickDockGroup);
    // d->layout();
}

QVariantList QQuickDockGroup::dockPanels() const
{
    Q_D(const QQuickDockGroup);
    QVariantList list;
    auto panels = d->tabBar->panels();
    for (auto &w : panels) {
        QVariantMap map;
        map.insert(QStringLiteral("title"), w->title());
        map.insert(QStringLiteral("icon"), w->icon().source());
        list << map;
    }
    return list;
}

Qt::Edge QQuickDockGroup::tabPosition() const
{
    Q_D(const QQuickDockGroup);
    return d->tabPosition;
}

void QQuickDockGroup::setTabPosition(Qt::Edge newTabPosition)
{
    Q_D(QQuickDockGroup);
    if (d->tabPosition == newTabPosition)
        return;
    d->tabPosition = newTabPosition;
    d->layout();
    emit tabPositionChanged();
}

QQuickDockGroup::QQuickDockGroup(QQuickDockGroupPrivate &dd, QQuickItem *parent)
    : QQuickContainer(dd, parent)
{
    Q_D(QQuickDockGroup);
    d->init();

    // setClip(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    // connect(this, &QQuickContainer::currentIndexChanged, this, &QQuickDockGroup::polish);
    QObjectPrivate::connect(this, &QQuickContainer::currentIndexChanged, d,
                            &QQuickDockGroupPrivate::currentIndexChanged);
}

bool QQuickDockGroup::isActive() const
{
    Q_D(const QQuickDockGroup);
    return d->isActive;
}

QT_END_NAMESPACE

#include "moc_qquickdockgroup_p.cpp"
