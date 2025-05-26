// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QQuickItem>

enum class ResizeDirection {
    Horizontal = Qt::Horizontal,
    Vertical = Qt::Vertical,
    Both = Horizontal | Vertical
};
Q_DECLARE_FLAGS(ResizeDirections, ResizeDirection)
Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeDirections)

enum class Edge { Top = 1, Left = 2, Right = 4, Bottom = 8 };
Q_DECLARE_FLAGS(Edges, Edge)
Q_DECLARE_OPERATORS_FOR_FLAGS(Edges)

class PaintCanvas : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QQuickItem *rubberBand READ rubberBand WRITE setRubberBand NOTIFY rubberBandChanged
                       FINAL)
    Q_PROPERTY(QQuickItem *activeItem READ activeItem WRITE setActiveItem NOTIFY activeItemChanged
                       FINAL)
    Q_PROPERTY(QQmlComponent *handleComponent READ handleComponent WRITE setHandleComponent NOTIFY
                       handleComponentChanged)

public:
    PaintCanvas(QQuickItem *parent = nullptr);

    QQuickItem *rubberBand() const;
    void setRubberBand(QQuickItem *newRubberBand);

    QQuickItem *activeItem() const;
    void setActiveItem(QQuickItem *newActiveItem);

    QQmlComponent *handleComponent() const;
    void setHandleComponent(QQmlComponent *newHandleComponent);

    Q_INVOKABLE void setPointer();
    Q_INVOKABLE void setTool(const QString &typeName, const QVariantMap &props);

protected:
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void createItem(const QRectF &rect);
    void rubberBandChanged();
    void activeItemChanged();
    void handleComponentChanged();

private:
    void setActiveItemRect(Edges edges, const QPointF &pt);

    struct HandleData
    {
        Qt::CursorShape cursor;
        ResizeDirection dur;
        Edges edge;
        QQuickItem *handle;
    };

    // clang-format off
    std::array<HandleData, 8> handleData = {{
        { Qt::SizeFDiagCursor,  ResizeDirection::Both,          Edge::Top | Edge::Left },
        { Qt::SizeVerCursor,    ResizeDirection::Vertical,      Edge::Top },
        { Qt::SizeBDiagCursor,  ResizeDirection::Both,          Edge::Top | Edge::Right },
        { Qt::SizeHorCursor,    ResizeDirection::Horizontal,    Edge::Right },
        { Qt::SizeFDiagCursor,  ResizeDirection::Both,          Edge::Right | Edge::Bottom },
        { Qt::SizeVerCursor,    ResizeDirection::Vertical,      Edge::Bottom },
        { Qt::SizeBDiagCursor,  ResizeDirection::Both,          Edge::Bottom | Edge::Left },
        { Qt::SizeHorCursor,    ResizeDirection::Horizontal,    Edge::Left },
    }};
    // clang-format on

    void updateHandlesGeometry();

    QString _typeName;
    QVariantMap _props;
    QQuickItem *m_rubberBand = nullptr;
    QQuickItem *m_activeItem = nullptr;
    QList<QQuickItem *> m_handles;
    QPointF _clickStartPos;
    QPointF _itemInitialPos;
    QQmlComponent *m_handleComponent = nullptr;
};
