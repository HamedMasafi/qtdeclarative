// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "ellipseshape.h"

#include <QPainter>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <cmath>

EllipseShape::EllipseShape(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true);
}

void EllipseShape::paint(QPainter *painter)
{
    // Enable antialiasing for smooth ellipse edges
    painter->setRenderHint(QPainter::Antialiasing, true);

    // Set the brush to fill the ellipse
    painter->setBrush(m_backgroundColor);

    // Set the pen for the border
    QPen pen(m_borderColor, m_borderWidth);
    painter->setPen(pen);

    // Adjust bounding rect to keep border inside
    QRectF rect = boundingRect().adjusted(m_borderWidth / 2.0, m_borderWidth / 2.0,
                                          -m_borderWidth / 2.0, -m_borderWidth / 2.0);

    // Draw the ellipse
    painter->drawEllipse(rect);
}

QColor EllipseShape::backgroundColor() const
{
    return m_backgroundColor;
}
QColor EllipseShape::borderColor() const
{
    return m_borderColor;
}
qreal EllipseShape::borderWidth() const
{
    return m_borderWidth;
}

void EllipseShape::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void EllipseShape::setBorderColor(const QColor &color)
{
    if (m_borderColor != color) {
        m_borderColor = color;
        emit borderColorChanged();
        update();
    }
}

void EllipseShape::setBorderWidth(qreal width)
{
    if (!qFuzzyCompare(m_borderWidth, width)) {
        m_borderWidth = width;
        emit borderWidthChanged();
        update();
    }
}
