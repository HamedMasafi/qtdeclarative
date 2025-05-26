// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQUICKDOCKTABBAR_P_P_H
#define QQUICKDOCKTABBAR_P_P_H

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

#include "qquickcontrol_p_p.h"
#include "qquickdocktabbar_p.h"

#include <qglobal.h>

class QQuickDockPanel;
class QQuickDockTabBarPrivate : public QQuickControlPrivate
{
    Q_DECLARE_PUBLIC(QQuickDockTabBar)

public:
    QList<QQuickDockPanel *> widgets;
    Qt::Edge position{ Qt::TopEdge };
    int currentIndex{ -1 };
    qreal implicitSize{ 40 };
};

#endif // QQUICKDOCKTABBAR_P_P_H
