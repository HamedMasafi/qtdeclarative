// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "basicshape.h"
#include <QQuickPaintedItem>
#include <QColor>

class RectangleShape : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                       backgroundColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

public:
    RectangleShape(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    QColor borderColor() const;
    void setBorderColor(const QColor &color);

    qreal borderWidth() const;
    void setBorderWidth(qreal width);

signals:
    void backgroundColorChanged();
    void borderColorChanged();
    void borderWidthChanged();

private:
    QColor m_backgroundColor = Qt::white;
    QColor m_borderColor = Qt::black;
    qreal m_borderWidth = 1.0;
};

REGISTER_SHAPE(RectangleShape)
