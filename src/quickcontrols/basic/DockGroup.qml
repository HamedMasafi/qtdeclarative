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

    topInset: control.tabPosition === Qt.TopEdge ? tabbar.height : 0
    leftInset: control.tabPosition === Qt.LeftEdge ? tabbar.width : 0
    bottomInset: control.tabPosition === Qt.BottomEdge ? tabbar.height : 0
    rightInset: control.tabPosition === Qt.RightEdge ? tabbar.width : 0

    topPadding: control.tabPosition === Qt.TopEdge ? 0 : 1
    leftPadding: control.tabPosition === Qt.LeftEdge ? 0 : 1
    bottomPadding: control.tabPosition === Qt.BottomEdge ? 0 : 1
    rightPadding: control.tabPosition === Qt.RightEdge ? 0 : 1

    spacing: 0

    background: Item {


        BorderLine {
            anchors.fill: parent
            visible: control.isActive
            // anchors.leftMargin: parent.leftInset
            // anchors.rightMargin: parent.rightInset
            // anchors.topMargin: parent.topInset
            // anchors.bottomMargin: parent.bottomInset
            color: control.palette.highlight

            topLine: control.tabPosition !== Qt.TopEdge
            leftLine: control.currentIndex !== Qt.LeftEdge
            rightLine: control.currentIndex !== Qt.RightEdge
            bottomLine: control.currentIndex !== Qt.BottomEdge
        }
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
}
