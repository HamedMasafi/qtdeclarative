// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Controls.Imagine
import QtQuick.Controls.Imagine.impl

T.DockPanel {
    id: control
    implicitHeight: 300
    implicitWidth: 250

    clip: true

    topPadding: resizeEdgeSize
    leftPadding: resizeEdgeSize
    rightPadding: resizeEdgeSize
    bottomPadding: resizeEdgeSize

    background: Rectangle {
        color: control.palette.window
        border.color: control.floating ? control.palette.mid : 'transparent'
        border.width: control.resizeEdgeSize
    }

    titleBar: Rectangle {
        color: control.palette.button

        implicitHeight: 30
        implicitWidth: 250
        clip: true

        NinePatchImage {
            anchors.fill: parent
            source: Imagine.url + "dockwidget-background"
            NinePatchImageSelector on source {
                states: [
                    {"disabled": !control.enabled}
                ]
            }
        }

        ToolButton {
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

        IconLabel {
            spacing: 6
            mirrored: control.mirrored
            display: IconLabel.TextBesideIcon

            anchors {
                left: parent.left
                leftMargin: 9
                verticalCenter: parent.verticalCenter
            }

            icon: control.icon
            text: control.title
            font: control.font
            color: control.isActive ? control.palette.highlight : control.palette.buttonText
        }

        Rectangle {
            color: control.palette.mid
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
        }
    }
}
