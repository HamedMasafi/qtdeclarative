// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKBORDERLINE_P_P_H
#define QQUICKBORDERLINE_P_P_H

#include <QColor>
#include <QtQuick/private/qquickpainteditem_p.h>

class QQuickBorderLinePrivate : public QQuickPaintedItemPrivate
{
public:
    QColor color{ Qt::black };
    QColor backgroundColor{ Qt::transparent };
    bool topLine{ true };
    bool leftLine{ true };
    bool rightLine{ true };
    bool bottomLine{ true };
    qreal topPadding{ 0 };
    qreal leftPadding{ 0 };
    qreal rightPadding{ 0 };
    qreal bottomPadding{ 0 };
};

#endif // QQUICKBORDERLINE_P_P_H
