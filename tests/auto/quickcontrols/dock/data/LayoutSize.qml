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
    property alias mainItem: mainItem

    property alias layout1: layout1

    property alias group1: group1
    property alias widget1: widget1

    property alias group2: group2
    property alias widget2: widget2

    // property alias layout1: layout1
    // property alias group1: group3

    DockContainer {
        id: container
        anchors.fill: parent

        popupType: DockContainer.Item
        DockLayout {
            id: mainLayout
            orientation: Qt.Vertical

            DockLayout {
                id: layout1
                orientation: Qt.Horizontal

                DockGroup{
                    id: group1
                    DockPanel {
                        id: widget1
                    }
                }
                Rectangle {
                    id: mainItem
                    color: window.palette.dark
                }

                DockGroup{
                    id: group2
                    DockPanel {
                        id: widget2
                    }
                }
            }
            DockLayout {
                id: layout2
                orientation: Qt.Horizontal
                DockGroup {
                    DockPanel {}
                    DockPanel {}
                    DockPanel {}
                }
            }
        }
    }
}
