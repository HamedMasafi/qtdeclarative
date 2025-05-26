// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockTabBar {
    id: control

    clip: true

    implicitWidth: implicitSize
    implicitHeight: implicitSize

    property bool isVertical: position == Qt.LeftEdge || position == Qt.RightEdge

    component Button : ToolButton {
        id: btn
        property real r: 0

        autoRepeat: true
        autoRepeatDelay: 400
        autoRepeatInterval: 80

        contentItem: IconLabel {
            rotation: btn.r
            icon.source: "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/arrow-indicator.png"
            defaultIconColor: control.visualFocus ? control.palette.highlight : control.palette.buttonText
        }
    }
    contentItem: Item {
        id: tabbar

        BorderLine {
            anchors.fill: parent
            topLine: control.position == Qt.BottomEdge
            leftLine: control.position == Qt.RightEdge
            rightLine: control.position == Qt.LeftEdge
            bottomLine: control.position == Qt.TopEdge
            color: control.palette.highlight
        }

        ListView {
            id: listView

            property bool showButtons: isVertical ? listView.height > tabbar.height : listView.width > tabbar.width
            property int pad: 0

            x: isVertical ? 0 : (showButtons ? pad : 0)
            y: isVertical ? (showButtons ? pad : 0) : 0

            model: control.dockPanels
            currentIndex: control.currentIndex
            width: isVertical ? control.implicitSize : contentWidth
            height: isVertical ? contentHeight : control.implicitSize

            spacing: 0
            orientation: control.position === Qt.LeftEdge || control.position === Qt.RightEdge ? ListView.Vertical : ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.AutoFlickIfNeeded
            snapMode: ListView.SnapToItem

            highlightMoveDuration: 0
            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: control.implicitSize
            preferredHighlightEnd: width - control.implicitSize

            delegate: Item {
                implicitHeight: isVertical ? btn.width : btn.height
                implicitWidth: isVertical ? btn.height : btn.width
                ToolButton {
                    id: btn

                    background: BorderLine {
                        anchors.fill: parent
                        color: control.palette.highlight
                        backgroundColor: btn.checked ? control.palette.window : (btn.hovered ? control.palette.highlight : 'transparent')

                        topLine: (control.position == Qt.BottomEdge) != (control.currentIndex === index)
                        bottomLine: (control.position == Qt.BottomEdge) != (control.currentIndex !== index)
                        leftLine: control.currentIndex === index
                        rightLine: control.currentIndex === index

                        bottomPadding: 1
                    }

                    implicitHeight: control.implicitSize

                    anchors.centerIn: parent
                    transform: Rotation {
                        origin.x: btn.width / 2
                        origin.y: btn.height / 2
                        angle: control.position === Qt.LeftEdge ? -90 :
                                                                  control.position === Qt.RightEdge ? 90 : 0
                    }

                    topInset: 0
                    bottomInset: 0
                    leftInset: 0
                    rightInset: 0

                    leftPadding: 10
                    rightPadding: 10

                    text: modelData.title
                    icon: modelData.icon
                    checkable: false
                    checked: control.currentIndex === index
                    onClicked: control.currentIndex = index
                }
            }
        }

        Rectangle {
            id: buttonsBox
            color: control.palette.window 
            visible: isVertical ? (listView.height > tabbar.height) : (listView.width > tabbar.width)
            width: isVertical ? tabbar.width : nextButton.width + prevButton.width
            height: isVertical ? nextButton.height + prevButton.height : tabbar.height

            x: isVertical ? 0 : parent.width - width
            y: isVertical ? parent.height - height : 0
            Button {
                id: prevButton
                r: isVertical ? -90 : 180
                onClicked: listView.pad = Math.min(listView.pad + 10, 0)
            }

            Button {
                id: nextButton

                y: isVertical ? prevButton.height + 1 : 0
                x: isVertical ? 0 : prevButton.width + 1
                r: isVertical ? 90 : 0
                onClicked: {
                    if (isVertical)
                        listView.pad = Math.max(listView.pad - 10, tabbar.height - buttonsBox.height - listView.height)
                    else
                        listView.pad = Math.max(listView.pad - 10, tabbar.width - buttonsBox.width - listView.width)
                }
            }
        }

    }
}
