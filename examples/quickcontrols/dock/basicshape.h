// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include <QQuickPaintedItem>
#include "factory.h"

class BasicShape : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged
                       FINAL)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                       backgroundColorChanged FINAL)
    Q_PROPERTY(
            int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged FINAL)

public:
    BasicShape(QQuickItem *parent = nullptr);

    QColor borderColor() const;
    void setBorderColor(const QColor &newBorderColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);

    int borderWidth() const;
    void setBorderWidth(int newBorderWidth);

signals:
    void borderColorChanged();

    void backgroundColorChanged();

    void borderWidthChanged();

private:
    QColor m_borderColor;
    QColor m_backgroundColor;
    int m_borderWidth;
};

#define REGISTER_SHAPE(Type)                                     \
    namespace {                                                  \
    struct AutoRegister_##Type                                   \
    {                                                            \
        AutoRegister_##Type() { Factory::registerType<Type>(); } \
    };                                                           \
    static AutoRegister_##Type instance_##Type;                  \
    }
