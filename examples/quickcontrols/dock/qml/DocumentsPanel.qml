// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import DockTest

Item {
    property alias canvas: canvas

    implicitHeight: 300
    implicitWidth: 300

    Factory {
        id: factory
        parentItem: canvas
    }
    Frame {
        anchors.fill: parent
        // anchors.margins: 9

        background: Rectangle{}
        PaintCanvas {
            id: canvas
            clip: true
            anchors.fill: parent

            rubberBand: Rectangle{
                border.color: 'blue'
                color: Qt.rgba(30, 180, 180, .5)
                z: 20
            }

            handleComponent: Rectangle {
                height: 10
                width: 10
                radius: 5
                border.color: 'black'
            }
        }
    }
}
