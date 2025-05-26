// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls
import DockTest

DockPanel {
    title: "Properties"

    property alias selectedItem: propManager.object

    PropertyManager {
        id: propManager
    }


    component ColorDelegate : ItemDelegate {
        width: ListView.view.width
        text: model.name
        ColorDialog {
            id: colorDialog
            onAccepted:  {
                propManager.setValue(model.name, selectedColor)
                colorRect.color = selectedColor
            }
        }
        contentItem: RowLayout {
            Label {
                text: model.name
            }
            Item {
                Layout.fillWidth: true
            }
            Rectangle {
                id: colorRect
                Layout.preferredWidth: 40
                Layout.preferredHeight: 28
                color: model.value
            }

            ToolButton {
                text: "..."
                onClicked: colorDialog.open()
            }
        }
    }
    component SizeDelegate : ItemDelegate {
        width: ListView.view.width
        text: model.name

        contentItem: RowLayout {
            Label {
                text: model.name
            }
            Item {
                Layout.fillWidth: true
            }
            SpinBox {
                value: model.value
               onValueModified: propManager.setValue(model.name, value)
            }
        }
    }

    contentItem: ListView {
        model: propManager
        delegate: DelegateChooser {

            role: "type"

            DelegateChoice {
                roleValue: "QColor"
                ColorDelegate {}
            }
            DelegateChoice {
                roleValue: "int"
                SizeDelegate {}
            }
            DelegateChoice {
                roleValue: "double"
                SizeDelegate {}
            }
        }
    }
}
