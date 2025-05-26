// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockPanel {
    id: control
    implicitHeight: 300
    implicitWidth: 250

    clip: true

    topPadding: resizeEdgeSize
    leftPadding: resizeEdgeSize
    rightPadding: resizeEdgeSize
    bottomPadding: resizeEdgeSize

    icon.width: 24
    icon.height: 24

    background: Rectangle {
        color: control.palette.window
        border.color: control.visibility == DockPanel.Pinned ? 'transparent' : control.palette.mid 
        border.width: control.resizeEdgeSize
    }

    titleBar: Rectangle {
        color: control.palette.button

        implicitHeight: 30
        implicitWidth: 250
        clip: true

        ToolButton {
            id: closeButton
            icon.source: "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/close.png"
            anchors {
                topMargin: 1
                bottomMargin: 1
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: 1
            }
            width: height
            onClicked: control.close()
        }
        ToolButton {
//            icon.source: control.pinned ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/unpin.png" :  "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/pin.png"
            anchors {
                topMargin: 1
                bottomMargin: 1
                top: parent.top
                bottom: parent.bottom
                right: closeButton.left
                rightMargin: 1
            }
            width: height
            onClicked: control.togglePin()
        }
        IconLabel {
            spacing: 6
            mirrored: control.mirrored
            display: IconLabel.TextBesideIcon

            x: 9
            height: parent.height

            icon: control.icon
             text: control.title
            font: control.font
            color: control.isActive ? control.palette.highlight : control.palette.buttonText
        }

        Rectangle {
            color: control.palette.highlight
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
        }
    }
}
