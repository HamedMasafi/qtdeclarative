// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef DOCKCONTAINER_P_H
#define DOCKCONTAINER_P_H

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

#include <QtQuickTemplates2/private/qquickdockcontainer_p.h>
#include <QtQuickTemplates2/private/qquickdocklayout_p_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p_p.h>
#include <QtQuick/private/qquickitem_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockMoveGuide;
class QQuickDockArea;
class QQuickDockLayout;
class QQuickDockGroup;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockContainerPrivate : public QQuickContainerPrivate
{

public:
    Q_DECLARE_PUBLIC(QQuickDockContainer)

    enum class InsertPos { None, Start, End };

    static QQuickDockContainerPrivate *get(QQuickDockContainer *item) { return item->d_func(); }

    static constexpr qreal zGroup{ 1 };
    static constexpr qreal zFloatingWidgetStart{ 10 };
    static constexpr qreal zFloatingWidgetEnd{ 99 };
    static constexpr qreal zMoveGuide{ 101 };
    static constexpr qreal zIndicatorItem{ 100 };

    QList<QQuickDockPanel *> dockPanels;
    QQuickDockPanel *activePanel{ nullptr };
    QQuickDockContainer::PopupType popupType{ QQuickDockContainer::PopupType::Native };

    QQuickDockArea *rightArea{ nullptr };
    QQuickDockArea *leftArea{ nullptr };
    QQuickDockArea *topArea{ nullptr };
    QQuickDockArea *bottomArea{ nullptr };

    QQmlComponent *splitViewCreationComponent{};
    QQmlComponent *widgetsGroupCreationComponent{};
    QQuickDockMoveGuide *dockMoveGuide;
    QQuickItem *dockIndicatorItem{ nullptr };
    QList<QQuickDockLayoutPrivate::DropAreaData> dropAreas;
    QList<QQuickDockLayoutPrivate::DropAreaData>::iterator currentDropArea;

    QQuickItem *dropArea{ nullptr };
    QRectF lastRect;
    QMap<Qt::Edge, int> areaSizes;

    QQuickDockLayout *rootSplitView = nullptr;

    QRectF contentRect;

    bool allowDetach{ false };

    QQuickItem *getContentItem() override;

    QQuickDockArea *area(Qt::Edge edge) const;

    QQuickDockLayout *createSplitView(QQuickItem *parent);
    QQuickDockGroup *createGroup(QQuickItem *parent);

    void changeDockArea(QQuickDockArea **oldArea, QQuickDockArea *newARea);

    void init(QQuickDockGroup *group);
    void init(QQuickDockLayout *layout);
    void init(QQuickDockPanel *widget);

    void addWidget(QQuickDockPanel *widget, Qt::Edge edge);

    void setPopupType(QQuickDockContainer::PopupType type);
    void storeAllDropAreas();
    void layout();

    void addItem(QQuickItem *item);

    void updatePolish() override;

    void setActiveWidget(QQuickDockPanel *widget);
    void setActiveItem(QQuickItem *item);
    void createDefaultLayout();
    QQuickDockContainer::PopupType resolvedPopupType() const;

    bool usePopupWindow() const;

    void connectWidgetSignals(QQuickDockPanel *widget);
    void dockPanelBeginMove();
    void dockPanelMoving(const QPointF &pt);
    void dockPanelMoved();
    void dockPanelVisibleChanged();
    void windowActiveFocusItemChanged();
    void sideTabBarCurrentIndexChanged();
    void areaCountChanged();
};

class QQuickDockContainerAttachedPrivate : QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockContainerAttached)

public:
    bool isFloating{ true };
    Qt::Edge edge{ Qt::RightEdge };
};

QT_END_NAMESPACE

#endif // DOCKCONTAINER_P_H
