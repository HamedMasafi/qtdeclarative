// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKAREA_P_H
#define QQUICKDOCKAREA_P_H

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

#include "qquickdockcontainer_p.h"
#include <QtQuickTemplates2/private/qquickcontrol_p.h>
#include <QtQuickTemplates2/private/qquickdockpanel_p.h>

#include <QQuickItem>

QT_BEGIN_NAMESPACE

class QQuickDockAreaPrivate;
class QQuickDockSideContainer;
class QQuickDockAreaResizeHandler;
class QQuickPopup;
class QQuickDockTabBar;

Q_DECLARE_OPAQUE_POINTER(QQuickDockSideContainer)
Q_DECLARE_OPAQUE_POINTER(QQuickPopup)

class Q_QUICKTEMPLATES2_EXPORT QQuickDockArea : public QQuickControl
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DockArea)
    QML_ADDED_IN_VERSION(6, 13)

    //TODO: remove this property
    Q_PROPERTY(int usedSize READ usedSize NOTIFY usedSizeChanged FINAL)
    Q_PROPERTY(QQuickDockSideContainer *sideContainer READ sideContainer WRITE setSideContainer
                       NOTIFY sideContainerChanged FINAL)
    Q_PROPERTY(QQuickPopup *popup READ popup WRITE setPopup NOTIFY popupChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(Qt::Edge tabPosition READ tabPosition WRITE setTabPosition NOTIFY tabPositionChanged
                       FINAL)
    Q_PROPERTY(QQuickDockTabBar *tabBar READ tabBar WRITE setTabBar NOTIFY tabBarChanged FINAL)
    Q_PROPERTY(QQuickItem *resizeHandle READ resizeHandle WRITE setResizeHandle NOTIFY
                       resizeHandleChanged FINAL)

    Q_DECLARE_PRIVATE(QQuickDockArea)

public:
    QQuickDockArea(QQuickItem *parent = nullptr);
    ~QQuickDockArea();

    void componentComplete() override;

    int usedSize() const;

    void addDockPanel(QQuickDockPanel *widget);
    void removeDockPanel(QQuickDockPanel *widget);

    QQuickDockSideContainer *sideContainer() const;
    void setSideContainer(QQuickDockSideContainer *newSideContainer);

    QQuickPopup *popup() const;
    void setPopup(QQuickPopup *newPopup);

    int count() const;

    Qt::Edge tabPosition() const;
    void setTabPosition(Qt::Edge newTabPosition);

    QList<QQuickDockPanel *> dockPanels() const;

    QQuickDockTabBar *tabBar() const;
    void setTabBar(QQuickDockTabBar *newTabBar);

    QQuickItem *resizeHandle() const;
    void setResizeHandle(QQuickItem *newResizeHandle);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();

Q_SIGNALS:
    void usedSizeChanged();
    void sideContainerChanged();
    void popupChanged();
    void countChanged();
    void tabPositionChanged();
    void dockPanelsChanged();
    void tabBarChanged();
    void resizeHandleChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

    Q_PRIVATE_SLOT(d_func(), void tabBarCurrentIndexChanged())
    Q_PRIVATE_SLOT(d_func(), void popupClosed())
    Q_PRIVATE_SLOT(d_func(), void sideContainerSizeChanged())
    Q_PRIVATE_SLOT(d_func(), void panelVisibilityChanged());
    Q_PRIVATE_SLOT(d_func(), void sideContainerPanelAdded(QQuickDockPanel *panel));
    Q_PRIVATE_SLOT(d_func(), void sideContainerPanelRemoved(QQuickDockPanel *panel));
};

QT_END_NAMESPACE

#endif // QQUICKDOCKAREA_P_H
