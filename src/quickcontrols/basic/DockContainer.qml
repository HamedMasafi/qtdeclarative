// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockContainer {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    dockIndicatorItem:  Rectangle {
        opacity: 0.4
        color: control.palette.highlight
        border.color: control.palette.highlightedText
        z: 100
    }
    background: Rectangle {
        color: control.palette.dark
    }

    moveGuide: DockMoveGuide {
        visible: false
        z: 101
    }

    splitView: DockLayout {}
    widgetsGroup: DockGroup { }

    leftArea: DockArea {
        tabPosition: Qt.LeftEdge
        implicitWidth: 40
    }
    rightArea: DockArea {
        tabPosition: Qt.RightEdge
        implicitWidth: 40
    }
    topArea: DockArea {
        tabPosition: Qt.TopEdge
        implicitHeight: 40
    }
    bottomArea: DockArea {
        tabPosition: Qt.BottomEdge
        implicitHeight: 40
    }
}
