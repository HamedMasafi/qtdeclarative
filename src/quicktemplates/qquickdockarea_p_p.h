// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKAREA_P_P_H
#define QQUICKDOCKAREA_P_P_H

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

#include "qquickdockarea_p.h"
#include "qquickdockgroup_p_p.h"
#include <QColor>
#include <QMap>
#include <QPointF>
#include <QRectF>
#include "qquickdockcontainer_p.h"
#include <QtQuickTemplates2/private/qquickcontrol_p_p.h>

QT_BEGIN_NAMESPACE

class QQuickItem;
class QQuickDockPanel;
class QQuickDockArea;
class QQuickDockAreaResizeHandler;
class QQuickDockTabBar;
class QQuickDockSideContainer;

class QQuickDockAreaPrivate : public QQuickControlPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockArea)

public:
    ~QQuickDockAreaPrivate();
    void itemParentChanged(QQuickItem *item, QQuickItem *parent) override;

    static QQuickDockAreaPrivate *get(QQuickDockArea *item) { return item->d_func(); }
    QQuickDockContainer *dockContainer{ nullptr };
    QQuickDockSideContainer *sideContainer{ nullptr };
    QList<QQuickDockPanel *> dockPanels;
    QQuickDockTabBar *tabBar{ nullptr };
    QQuickPopup *popup{ nullptr };
    Qt::Edge tabPosition{ Qt::LeftEdge };
    QQuickItem *resizeHandle{ nullptr };
    bool openGuard{ false };

    void addDockPanel(QQuickDockPanel *widget);
    void removeDockPanel(QQuickDockPanel *widget);
    void updatePolish() override;
    void layout();
    void updateResizeEdge();
    QQuickItem *getContentItem() override;

    void tabBarCurrentIndexChanged();
    void popupClosed();
    void sideContainerSizeChanged();
    void itemRemoved(int index, QQuickItem *item);
    void panelVisibilityChanged();
    void sideContainerPanelAdded(QQuickDockPanel *panel);
    void sideContainerPanelRemoved(QQuickDockPanel *panel);
};

QT_END_NAMESPACE

#endif // QQUICKDOCKAREA_P_P_H
