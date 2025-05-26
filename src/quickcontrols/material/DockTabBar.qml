// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

T.DockTabBar {
    id: control

    clip: true

    implicitWidth: 40
    implicitHeight: 40

    contentItem: Item {
        id: tabbar

        clip: true

        BorderLine {
            anchors.fill: parent
            topLine: true
            leftLine: false
            rightLine: false
            bottomLine: false
            color: control.Material.frameColor
        }

        ListView {
            model: control.dockPanels
            currentIndex: control.currentIndex
            anchors.fill: parent

            spacing: control.spacing
            orientation: control.position === Qt.LeftEdge || control.position === Qt.RightEdge ? ListView.Vertical : ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickIfNeeded
            snapMode: ListView.SnapToItem

            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: 40
            preferredHighlightEnd: width - 40

            delegate: Item {
                visible: modelData.isOpen
                implicitHeight: control.position === Qt.LeftEdge || control.position === Qt.RightEdge ? btn.width : btn.height
                implicitWidth: control.position === Qt.LeftEdge || control.position === Qt.RightEdge ? btn.height : btn.width
                ToolButton {
                    id: btn

                    implicitHeight: control.implicitHeight

                    anchors.centerIn: parent
                    transform: Rotation {
                        origin.x: btn.width / 2
                        origin.y: btn.height / 2
                        angle: control.position === Qt.LeftEdge ? -90 :
                                                                  control.position === Qt.RightEdge ? 90 : 0
                    }


                    background: Ripple {
                        implicitWidth: control.Material.touchTarget
                        implicitHeight: control.Material.touchTarget

                        x: (parent.width - width) / 2
                        y: (parent.height - height) / 2
                        clip: true
                        width: parent.width
                        height:  parent.height
                        pressed: btn.pressed
                        anchor: control
                        active: btn.enabled && (btn.down || btn.visualFocus || btn.hovered)
                        color: control.Material.rippleColor
                        Rectangle {
                            anchors.fill: parent
                            color: btn.checked ? control.Material.background : (btn.hovered ? control.Material.listHighlightColor : 'transparent')
                        }
                    }
                    BorderLine {
                        anchors.fill: parent
                        color: control.Material.frameColor

                        topLine: control.currentIndex !== index
                        leftLine: control.currentIndex === index
                        rightLine: control.currentIndex === index
                        bottomLine: control.currentIndex === index

                        bottomPadding: 1
                    }

                    leftPadding: 10
                    rightPadding: 10
                    topPadding: 10
                    bottomPadding: 10

                    text: modelData.title
                    icon: modelData.icon
                    checkable: false
                    checked: control.currentIndex === index
                    onClicked: control.currentIndex = index
                }
            }
        }
    }
}
