// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKGROUP_P_P_H
#define QQUICKDOCKGROUP_P_P_H

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

#include <QColor>
#include <QMap>
#include <QPointF>
#include <QRectF>
#include <QtQuickTemplates2/private/qquickdockgroup_p.h>
#include <QtQuickTemplates2/private/qquickdockcontainer_p.h>
#include <QtQuickTemplates2/private/qquickcontainer_p_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockTabBar;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockGroupPrivate : public QQuickContainerPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockGroup)

public:
    ~QQuickDockGroupPrivate();
    static QQuickDockGroupPrivate *get(QQuickDockGroup *area);

    QQuickItem *getContentItem() override;
    void itemParentChanged(QQuickItem *item, QQuickItem *parent) override;

    bool layingOut{ false };
    bool isActive{ false };

    QList<QQuickDockPanel *> detachedDockPanels;
    QQuickDockTabBar *tabBar{ nullptr };
    QQuickDockContainer *dockContainer{ nullptr };
    Qt::Edge tabPosition{ Qt::BottomEdge };
    QQuickDockPanel *activePanel{ nullptr };

    void updatePolish() override;
    void setActive(bool active);
    void layout();
    void currentIndexChanged();

    void addDockPanel(QQuickDockPanel *widget);
    void removeDockPanel(QQuickDockPanel *widget);
    void detachDockPanel(QQuickDockPanel *widget);

    void checkForCount();

    void widgetActiveChanged();
    void tabbarCountChanged();
    void tabbarCurrentIndexChanged();
    void panelVisibilityChanged();
};

QT_END_NAMESPACE

#endif // QQUICKDOCKGROUP_P_P_H
