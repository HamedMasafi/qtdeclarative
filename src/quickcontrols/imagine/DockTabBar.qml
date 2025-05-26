// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockTabBar {
    id: control

    clip: true

    implicitWidth: 40
    implicitHeight: 40

    contentItem: Item {
        id: tabbar

        clip: true

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

                    background: NinePatchImage {
                        source: Imagine.url + "docktabbar-button"
                        NinePatchImageSelector on source {
                            states: [
                                // {"down": control.position !== Qt.BottomEdge},
                                {"checked": btn.checked},
                                {"hovered": btn.hovered},
                                {"pressed": btn.down}

                                // {"focused": control.visualFocus},
                                // {"highlighted": control.highlighted},
                                // {"mirrored": control.mirrored},
                            ]
                        }
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
