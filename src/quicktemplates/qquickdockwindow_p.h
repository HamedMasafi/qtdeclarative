// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef DOCKWINDOW_H
#define DOCKWINDOW_H

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

#include <QQuickWindow>
#include "qtquicktemplates2exports.h"

class Q_QUICKTEMPLATES2_EXPORT QQuickDockWindow : public QQuickWindow
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DockWindow)

    QQuickItem *_dockPanels;

public:
    QQuickDockWindow(QWindow *parent = nullptr);

    QQuickItem *dockPanel() const;
    void setDockPanel(QQuickItem *dockPanel);

protected:
    void resizeEvent(QResizeEvent *) override;

    // QObject interface
public:
    bool event(QEvent *event) override;

protected:
    void moveEvent(QMoveEvent *) override;
};

#endif // DOCKWINDOW_H
