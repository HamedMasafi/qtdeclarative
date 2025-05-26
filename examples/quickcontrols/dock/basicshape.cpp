// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "basicshape.h"

BasicShape::BasicShape(QQuickItem *parent) : QQuickPaintedItem{ parent } { }

QColor BasicShape::borderColor() const
{
    return m_borderColor;
}

void BasicShape::setBorderColor(const QColor &newBorderColor)
{
    if (m_borderColor == newBorderColor)
        return;
    m_borderColor = newBorderColor;
    emit borderColorChanged();
}

QColor BasicShape::backgroundColor() const
{
    return m_backgroundColor;
}

void BasicShape::setBackgroundColor(const QColor &newBackgroundColor)
{
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_backgroundColor = newBackgroundColor;
    emit backgroundColorChanged();
}

int BasicShape::borderWidth() const
{
    return m_borderWidth;
}

void BasicShape::setBorderWidth(int newBorderWidth)
{
    if (m_borderWidth == newBorderWidth)
        return;
    m_borderWidth = newBorderWidth;
    emit borderWidthChanged();
}
