// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window

    width: 640
    height: 480
    visible: true

    property alias container: container
    property alias widgetLeft: widgetLeft
    property alias widgetTop: widgetTop
    property alias widgetRight: widgetRight
    property alias widgetBottom: widgetBottom

    DockContainer {
        id: container
        anchors.fill: parent

       popupType: DockContainer.Item
       DockLayout {
            id: layout1
            orientation: Qt.Horizontal

            DockGroup{
                DockPanel {
                    id: widgetLeft
                    visibility: DockPanel.Unpinned
                    edge: Qt.LeftEdge
                }
                DockPanel {
                    id: widgetRight
                    visibility: DockPanel.Unpinned
                    edge: Qt.RightEdge
                }
                DockPanel {
                    id: widgetTop
                    visibility: DockPanel.Unpinned
                    edge: Qt.TopEdge
                }
                DockPanel {
                    id: widgetBottom
                    visibility: DockPanel.Unpinned
                    edge: Qt.BottomEdge
                }
            }
        }
    }
}

