// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

DockPanel {
    title: "Export"

    contentItem: Item {
        id: root
        width: 400
        height: 150

        FileDialog {
            id: fileDialog
            title: "Choose export file"
            fileMode: FileDialog.SaveFile
            onAccepted: {
                filePathField.text = fileDialog.selectedFile
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            anchors.margins: 10

            Label {
                text: "Select export location:"
                Layout.alignment: Qt.AlignLeft
            }

            RowLayout {
                Layout.fillWidth: true

                TextField {
                    id: filePathField
                    Layout.fillWidth: true
                    placeholderText: "Export file path..."
                }

                ToolButton {
                    text: "..."
                    onClicked: fileDialog.open()
                }
            }

            Button {
                text: "Export"
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    // Export logic goes here
                    console.log("Exporting to:", filePathField.text)
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
