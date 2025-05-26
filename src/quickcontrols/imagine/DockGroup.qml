// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockGroup {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    topPadding: control.tabPosition === Qt.TopEdge ? tabbar.height : 1
    leftPadding: control.tabPosition === Qt.LeftEdge ? tabbar.width : 1
    bottomPadding: control.tabPosition === Qt.BottomEdge ? tabbar.height : 1
    rightPadding: control.tabPosition === Qt.RightEdge ? tabbar.width : 1

    spacing: 0

    contentItem: Item {
        clip: true

        implicitHeight: 200
        implicitWidth: 300
    }
    tabBar: DockTabBar {
        id: tabbar
        height: 40

        onCurrentIndexChanged: control.currentIndex = currentIndex
        states: [
            State {
                name: "top"
                when: control.tabPosition === Qt.TopEdge
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: 0
                    width: control.width
                    height: 40
                }
            },
            State {
                name: "left"
                when: control.tabPosition === Qt.LeftEdge
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: 0
                    width: 40
                    height: control.width
                }
            },
            State {
                when: control.tabPosition === Qt.RightEdge
                name: "right"
                PropertyChanges {
                    target: tabBar
                    x: control.width - 40
                    y: 0
                    width: 40
                    height: control.height
                }
            },
            State {
                when: control.tabPosition === Qt.BottomEdge
                name: "bottom"
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: control.height - 40
                    width: control.width
                    height: 40
                }
            }
        ]
    }
/*
    tabBar: Item {
        id: tabbar
        height: 40
        width: control.width
        z: 10

        property int size: 40
        clip: true

        states: [
            State {
                name: "top"
                when: control.tabPosition === Qt.TopEdge
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: 0
                    width: control.width
                    height: 40
                }
            },
            State {
                name: "left"
                when: control.tabPosition === Qt.LeftEdge
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: 0
                    width: 40
                    height: control.width
                }
            },
            State {
                when: control.tabPosition === Qt.RightEdge
                name: "right"
                PropertyChanges {
                    target: tabBar
                    x: control.width - 40
                    y: 0
                    width: 40
                    height: control.height
                }
            },
            State {
                when: control.tabPosition === Qt.BottomEdge
                name: "bottom"
                PropertyChanges {
                    target: tabBar
                    x: 0
                    y: control.height - 40
                    width: control.width
                    height: 40
                }
            }
        ]

        ListView {
            model: control.dockWidgets
            currentIndex: control.currentIndex
            anchors.fill: parent

            spacing: 0
            orientation: control.tabPosition === Qt.LeftEdge || control.tabPosition === Qt.RightEdge ? ListView.Vertical : ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickIfNeeded
            snapMode: ListView.SnapToItem

            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: 40
            preferredHighlightEnd: width - 40

            delegate: Item {
                height: control.tabPosition === Qt.LeftEdge || control.tabPosition === Qt.RightEdge ? btn.width : btn.height
                width: control.tabPosition === Qt.LeftEdge || control.tabPosition === Qt.RightEdge ? btn.height : btn.width
                TabButton {
                    id: btn

                    anchors.centerIn: parent
                    transform: Rotation {
                        origin.x: btn.width / 2
                        origin.y: btn.height / 2
                        angle: control.tabPosition === Qt.LeftEdge ? -90 :
                                                                     control.tabPosition === Qt.RightEdge ? 90 : 0
                    }

                    leftPadding: 10
                    rightPadding: 10
                    topPadding: 10
                    bottomPadding: 10
                    text: modelData.title
                    checked: control.currentIndex === index
                    onClicked: control.currentIndex = index
                }
            }
        }
    }
*/
}
