// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKPANEL_P_P_H
#define QQUICKDOCKPANEL_P_P_H

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
#include "qquickdockpanel_p.h"
#include <QtQuickTemplates2/private/qquickcontrol_p_p.h>

#include <QRectF>
#include <QSizeF>

class QQuickDockWindow;
class QQuickItem;
class QQuickDockContainer;
class QQuickDockArea;
class QQuickDockGroup;
class Q_QUICKTEMPLATES2_EXPORT QQuickDockPanelPrivate : public QQuickControlPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockPanel)

public:
    QQuickDockPanelPrivate();

    static QQuickDockPanelPrivate *get(QQuickDockPanel *widget) { return widget->d_func(); }

    QQuickItem *titleBarItem{ nullptr };
    QQuickDockWindow *dockWindow{ nullptr };
    QQuickDockContainer *dockContainer{ nullptr };
    QQuickIcon icon;

    QSizeF originalSize;

    bool closable;
    bool resizable;
    bool movable;
    bool showHeader;

    QString title;

    // state
    bool isClosed{ false };
    Qt::Edge edge{ Qt::RightEdge };
    QQuickDockGroup *group{ nullptr };
    QQuickDockGroup *homeGroup{ nullptr };
    QQuickDockPanel::Visibility oldVisibility{ QQuickDockPanel::Visibility::Closed };
    QQuickDockPanel::Visibility visibility{ QQuickDockPanel::Visibility::Pinned };
    QRectF floatingGeometry;

    void setInitState();
    void unpin();
    void pinTo(QQuickDockGroup *group = nullptr);
    void detach();
    void open();
    void close();

    // resize
    int resizeEdge;
    int resizeEdgeSize{ 2 };
    QRectF rectBeforeResize{ 0, 0, 300, 300 };
    QPointF resizeMousePos;

    QPointF moveClickPos;
    QPointF moveStartPos;

    bool isActive{ false };
    bool isOpen{ true };
    // bool isFloaing{ false };
    // bool isDetached{ false };
    bool isWayland{ false };

    void resizeContent() override;
    void relayout();

    void itemGeometryChanged(QQuickItem *item, QQuickGeometryChange change,
                             const QRectF &diff) override;
    void itemVisibilityChanged(QQuickItem *item) override;
    void resizeToRect(const QPointF &pt);

    void setVisibility(QQuickDockPanel::Visibility visibility);
    void setEdge(Qt::Edge);

    void detachFromOldParent();

    void headerMoveStarted(QMouseEvent *event);
    void headerMoving(QMouseEvent *event);
    void headerMoveEnded();
    void headerDoubleClicked();

    void setActive(bool active);
    void attach();
    void createDockWindow();
};

#endif // QQUICKDOCKPANEL_P_P_H
