// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKSIDECONTAINER_P_P_H
#define QQUICKDOCKSIDECONTAINER_P_P_H

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

#include <QtQuickTemplates2/private/qquickcontainer_p_p.h>

class QQuickDockPanel;
class QQuickDockSideContainerAttached;
class QQuickDockSideContainerAttachedPrivate;
class QQuickDockSideContainerPrivate : public QQuickContainerPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockSideContainer)
public:
    QQuickItem *resizeHandle{ nullptr };
    QQuickDockSideContainerAttached *handleAttached{ nullptr };
    QQuickDockSideContainerAttachedPrivate *handleAttachedPrivate{ nullptr };

    QList<QQuickDockPanel *> dockPanels;
    Qt::Edge resizeEdge{ Qt::RightEdge };
    int size;

    QPointF clickPos;
    int startSize;

    QQuickItem *getContentItem() override;

    void addDockPanel(QQuickDockPanel *widget);
    void removeDockPanel(QQuickDockPanel *widget);
    void updateSize();
    void layout();
    bool handlePress(const QPointF &point, ulong timestamp) override;
    bool handleMove(const QPointF &point, ulong timestamp) override;
    void updateResizeHandlerCursor();
    void updatePolish() override;
    void itemParentChanged(QQuickItem *item, QQuickItem *parent) override;
};

class QQuickDockSideContainerAttachedPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockSideContainerAttached)

public:
    QQuickDockSideContainerAttachedPrivate();

    static QQuickDockSideContainerAttachedPrivate *get(QQuickDockSideContainerAttached *attached);
    static const QQuickDockSideContainerAttachedPrivate *
    get(const QQuickDockSideContainerAttached *attached);

    void setHovered(bool hovered);
    void setPressed(bool pressed);

    bool m_hovered{ false };
    bool m_pressed{ false };
};

#endif // QQUICKDOCKSIDECONTAINER_P_P_H
