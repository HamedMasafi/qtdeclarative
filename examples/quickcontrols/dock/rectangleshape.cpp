// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "rectangleshape.h"

#include <QDebug>
#include <QPainter>
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>

RectangleShape::RectangleShape(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(ItemHasContents, true); // Necessary to trigger paint node
}

void RectangleShape::paint(QPainter *painter)
{
    // Set antialiasing for smoother edges (optional)
    painter->setRenderHint(QPainter::Antialiasing, true);

    // Set brush for background
    painter->setBrush(m_backgroundColor);

    // Set pen for border
    QPen pen(m_borderColor, m_borderWidth);
    painter->setPen(pen);

    // Adjust the rectangle to account for half the pen width,
    // so the border stays within the bounding rect
    QRectF rect = boundingRect().adjusted(m_borderWidth / 2.0, m_borderWidth / 2.0,
                                          -m_borderWidth / 2.0, -m_borderWidth / 2.0);

    // Draw the rectangle
    painter->drawRect(rect);
}

QColor RectangleShape::backgroundColor() const
{
    return m_backgroundColor;
}
QColor RectangleShape::borderColor() const
{
    return m_borderColor;
}
qreal RectangleShape::borderWidth() const
{
    return m_borderWidth;
}

void RectangleShape::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

void RectangleShape::setBorderColor(const QColor &color)
{
    if (m_borderColor != color) {
        m_borderColor = color;
        emit borderColorChanged();
        update();
    }
}

void RectangleShape::setBorderWidth(qreal width)
{
    if (!qFuzzyCompare(m_borderWidth, width)) {
        m_borderWidth = width;
        emit borderWidthChanged();
        update();
    }
}
