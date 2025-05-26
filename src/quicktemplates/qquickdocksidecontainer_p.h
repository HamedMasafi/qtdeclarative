// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKSIDECONTAINER_P_H
#define QQUICKDOCKSIDECONTAINER_P_H

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

class QQuickDockSideContainerPrivate;
class QQuickDockSideContainerAttachedPrivate;
class QQuickDockPanel;
class QQuickDockSideContainer : public QQuickContainer
{
    Q_OBJECT

    Q_PROPERTY(QQuickItem *resizeHandle READ resizeHandle WRITE setResizeHandle NOTIFY
                       resizeHandleChanged FINAL)
    Q_PROPERTY(
            Qt::Edge resizeEdge READ resizeEdge WRITE setResizeEdge NOTIFY resizeEdgeChanged FINAL)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged FINAL)

    QML_NAMED_ELEMENT(DockSideContainer)
    QML_ADDED_IN_VERSION(6, 13)

    Q_DECLARE_PRIVATE(QQuickDockSideContainer)

public:
    explicit QQuickDockSideContainer(QQuickItem *parent = nullptr);

    void componentComplete() override;

    QQuickItem *resizeHandle() const;
    void setResizeHandle(QQuickItem *newResizeHandle);
    Qt::Edge resizeEdge() const;
    void setResizeEdge(Qt::Edge newResizeEdge);

    int size() const;
    void setSize(int newSize);

Q_SIGNALS:
    void resizeHandleChanged();
    void resizeEdgeChanged();
    void sizeChanged();
    void panelAdded(QQuickDockPanel *panel);
    void panelRemoved(QQuickDockPanel *panel);

protected:
    void itemAdded(int index, QQuickItem *item) override;
    void itemRemoved(int index, QQuickItem *item) override;
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

    Q_PRIVATE_SLOT(d_func(), void layout())
};

class QQuickDockSideContainerAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged FINAL)
    Q_PROPERTY(bool pressed READ isPressed NOTIFY pressedChanged FINAL)
    QML_NAMED_ELEMENT(SideContainerHandle)
    QML_ATTACHED(QQuickDockSideContainerAttached)
    QML_UNCREATABLE("")
    QML_ADDED_IN_VERSION(6, 13)

public:
    explicit QQuickDockSideContainerAttached(QObject *parent = nullptr);

    bool isHovered() const;
    bool isPressed() const;

    static QQuickDockSideContainerAttached *qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void hoveredChanged();
    void pressedChanged();

private:
    Q_DISABLE_COPY(QQuickDockSideContainerAttached)
    Q_DECLARE_PRIVATE(QQuickDockSideContainerAttached)
};

QT_END_NAMESPACE

#endif // QQUICKDOCKSIDECONTAINER_P_H
