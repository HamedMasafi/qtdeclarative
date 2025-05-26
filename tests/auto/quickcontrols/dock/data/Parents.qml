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
    property alias widget1: widget1
    property alias widget2: widget2 
    property alias widget3: widget3 
    property alias widget4: widget4
    property alias group1: group1
    DockContainer {
        id: container
        anchors.fill: parent

       popupType: DockContainer.Item
       DockLayout {
            id: layout1
            orientation: Qt.Horizontal

            DockGroup{
                id: group1
                DockPanel {
                    id: widget1
                }

                DockPanel {
                    id: widget2
                    visibility: DockPanel.Unpinned
                    edge: Qt.TopEdge
                }
                DockPanel {
                    id: widget3
                    visibility: DockPanel.Detached
                }
                DockPanel {
                    id: widget4
                    visibility: DockPanel.Closed
                }
            }
        }
    }
}

