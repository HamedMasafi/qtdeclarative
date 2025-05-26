// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import QtQuick.Templates as T

T.DockPanel {
    id: control

    implicitHeight: 300
    implicitWidth: 250

    bottomPadding: resizeEdgeSize
    leftPadding: resizeEdgeSize
    rightPadding: resizeEdgeSize
    topPadding: resizeEdgeSize

    Material.elevation: control.visibility === DockPanel.Detached ? 8 : 0
    Material.roundedScale: Material.FullScale

    background: Rectangle {
        border.color:  control.visibility === DockPanel.Detached ? control.Material.dockWidgetBorderColor : 'transparent'
        border.width: control.resizeEdgeSize
        color: control.Material.background
    }

    titleBar: Rectangle {
        clip: true

        implicitHeight: 40//control.Material.dockWidgetTitleBarHeight
        implicitWidth: 250

        color: control.Material.dockWidgetTitleBarBackgroundColor

        ToolButton {
            id: closeButton
            icon{
                source: "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/close.png"
                color: control.Material.dockWidgetTitleLabelColor
            }
            width: height

            onClicked: control.close()

            anchors {
                bottom: parent.bottom
                bottomMargin: 1
                right: parent.right
                rightMargin: 1
                top: parent.top
                topMargin: 1
            }
        }
        ToolButton {
            icon{
                source: control.pinned ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/unpin.png" :  "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/pin.png"
                color: control.Material.dockWidgetTitleLabelColor
            }

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
            color: control.Material.dockWidgetTitleLabelColor
            display: IconLabel.TextBesideIcon
            font: control.font
            icon: control.icon
            mirrored: control.mirrored
            spacing: 6
            text: control.title

            anchors {
                left: parent.left
                leftMargin: 9
                verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            color: control.Material.accentColor
            height: 1

            anchors {
                bottom: parent.bottom
                left: parent.left
                top: parent.top
            }
        }
    }
}
