// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockArea {
    id: control

    clip: true

    implicitWidth: 40//Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            // contentWidth + leftPadding + rightPadding)
    implicitHeight: 40//Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             // contentHeight + topPadding + bottomPadding)

    // topPadding: control.tabPosition === Qt.TopEdge ? tabbar.size : 1
    // leftPadding: control.tabPosition === Qt.LeftEdge ? tabbar.size : 1
    // bottomPadding: control.tabPosition === Qt.BottomEdge ? tabbar.size : 1
    // rightPadding: control.tabPosition === Qt.RightEdge ? tabbar.size : 1

    tabBar: DockTabBar {
        // implicitHeight: 40
        // implicitWidth: 40
        clip: true
    }

    sideContainer: DockSideContainer{}

    popup: Popup {
        topPadding: 0
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
    }
}
