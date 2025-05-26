// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockSideContainer {
    id: control
    implicitHeight: 300
    implicitWidth: 250

    topInset: 0
    rightPadding: 0
    bottomPadding: 0
    leftPadding: 0

    resizeHandle: Rectangle {
        implicitHeight: 4
        implicitWidth: 4
        color: T.SideContainerHandle.pressed ? control.palette.mid
            : (T.SideContainerHandle.hovered ? control.palette.midlight : 'transparent')
    }

    background: Rectangle {
        color: control.palette.window
        border.color: control.palette.mid
    }
}
