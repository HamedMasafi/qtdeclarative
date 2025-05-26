// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKGROUP_P_H
#define QQUICKDOCKGROUP_P_H

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

#include <QtQuickTemplates2/private/qquickcontainer_p.h>

QT_BEGIN_NAMESPACE

class QQuickDockPanel;
class QQuickDockGroupPrivate;
class QQuickDockTabBar;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockGroup : public QQuickContainer
{
    Q_OBJECT

    Q_PROPERTY(QQuickDockPanel *activePanel READ activePanel NOTIFY activePanelChanged)
    Q_PROPERTY(QQuickDockTabBar *tabBar READ tabBar WRITE setTabBar NOTIFY tabBarChanged FINAL)
    Q_PROPERTY(QVariantList dockPanels READ dockPanels NOTIFY dockPanelsChanged FINAL)
    Q_PROPERTY(Qt::Edge tabPosition READ tabPosition WRITE setTabPosition NOTIFY tabPositionChanged
                       FINAL)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged FINAL)

    QML_NAMED_ELEMENT(DockGroup)
    QML_ADDED_IN_VERSION(6, 13)

    Q_DECLARE_PRIVATE(QQuickDockGroup)

public:
    explicit QQuickDockGroup(QQuickItem *parent = nullptr);
    ~QQuickDockGroup();

    QQuickDockPanel *activePanel() const;

    [[nodiscard]] QQuickDockTabBar *tabBar() const;
    void setTabBar(QQuickDockTabBar *newTabBar);

    [[nodiscard]] QVariantList dockPanels() const;

    [[nodiscard]] Qt::Edge tabPosition() const;
    void setTabPosition(Qt::Edge newTabPosition);

    [[nodiscard]] bool isActive() const;

Q_SIGNALS:
    void tabBarChanged();
    void dockPanelsChanged();
    void tabPositionChanged();
    void isActiveChanged();
    void activePanelChanged();

protected:
    QQuickDockGroup(QQuickDockGroupPrivate &dd, QQuickItem *parent);

    void updatePolish() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void itemAdded(int index, QQuickItem *item) override;
    void itemRemoved(int index, QQuickItem *item) override;
    bool isContent(QQuickItem *item) const override;

    Q_PRIVATE_SLOT(d_func(), void currentIndexChanged())
    Q_PRIVATE_SLOT(d_func(), void tabbarCountChanged())
    Q_PRIVATE_SLOT(d_func(), void tabbarCurrentIndexChanged())
    Q_PRIVATE_SLOT(d_func(), void panelVisibilityChanged());
};

QT_END_NAMESPACE

#endif // QQUICKDOCKGROUP_P_H
