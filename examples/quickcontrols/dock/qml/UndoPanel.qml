// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

DockPanel {
    title: "Undo stack"

    ListModel {
        id: undoModel
        ListElement {
            title: "Move element"
        }
        ListElement {
            title: "Change properties"
        }
        ListElement {
            title: "Create element"
        }
        ListElement {
            title: "New document"
        }
    }

    contentItem: ColumnLayout {

        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                ToolButton {

                }
                ToolButton {

                }
            }
        }

        ListView {
            model: undoModel
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 9
            delegate: Label {
                text: title
            }
        }
    }
}
