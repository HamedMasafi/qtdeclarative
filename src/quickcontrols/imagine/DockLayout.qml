// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl

T.DockLayout {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    handle: NinePatchImage {
        source: Imagine.url + "splitview-handle"
        NinePatchImageSelector on source {
            states: [
                {"vertical": control.orientation === Qt.Vertical},
                {"horizontal":control.orientation === Qt.Horizontal},
                {"disabled": !control.enabled},
                {"pressed": T.DockLayoutHandle.pressed},
                {"mirrored": control.mirrored},
                {"hovered": T.DockLayoutHandle.hovered}
            ]
        }
    }
}
