// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Kaj.Dock 1.0

DockPanel {
    area: Dock.Left
    title: "Custom title"

    titleBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: title
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                text: "x"
                onClicked: close()
            }
        }
    }

    contentItem: Label {
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("I have custom title bar")
    }
}
