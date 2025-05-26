// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.qmlmodels
import Qt.labs.folderlistmodel
import DockTest

DockPanel {
    icon.source: 'qrc:/res/folder.png'

    property PaintCanvas canvas

    clip: false
    property var typesModel: [
        {
            name: "Red rectangle",
            type: "RectangleShape",
            props: {
                backgroundColor: "red",
                borderColor: "black",
                borderWidth: 2
            }
        },
        {
            name: "Blue rectangle",
            type: "RectangleShape",
            props: {
                backgroundColor: "blue",
                borderColor: "black",
                borderWidth: 2
            }
        },
        {
            name: "Green rectangle",
            type: "RectangleShape",
            props: {
                backgroundColor: "green",
                borderColor: "black",
                borderWidth: 2
            }
        },
        {
            name: "Red circle",
            type: "EllipseShape",
            props: {
                backgroundColor: "red",
                borderColor: "black",
                borderWidth: 1
            }
        },
        {
            name: "Green circle",
            type: "EllipseShape",
            props: {
                backgroundColor: "green",
                borderColor: "black",
                borderWidth: 1
            }
        },
        {
            name: "Blue circle",
            type: "EllipseShape",
            props: {
                backgroundColor: "blue",
                borderColor: "black",
                borderWidth: 1
            }
        }
    ]


    Factory {
        id: factory
    }

    contentItem:  ListView {
        model: typesModel
        delegate: ItemDelegate {
            width: ListView.view.width

            onClicked: {
                canvas.setTool(modelData.type, modelData.props)
                window.selectedTool = modelData
            }

            contentItem: RowLayout {
                Item {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 30
                    id: cell
                    Component.onCompleted: {
                        var props = modelData.props
                        props.width = width
                        props.height = height
                        factory.create(cell, modelData.type, props)
                    }
                }
                Label {
                    text: modelData.name
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                }
            }
        }
    }
    /*contentItem: ScrollView {
        id: scrollView

        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        // Required: Only Flickable content is allowed

        /*contentItem: Flickable {
            id: flickable
            contentWidth: flow.implicitWidth
            contentHeight: flow.implicitHeight
            clip: true

            Flow {
                id: flow
                spacing: 10
                width: flickable.width  // Prevent horizontal scrolling

                Repeater {
                    model: typesModel
                    delegate: Item {
                        id: cell
                        width: 50
                        height: 50

                        property Item preview: null
                        Component.onCompleted: {
                            var props = modelData.props
                            props.width = width
                            props.height = height
                            preview = factory.create(cell, modelData.type, props)
                        }

                        MouseArea {
                            id: dragArea
                            anchors.fill: parent

                            onClicked: {
                                canvas.setTool(modelData.type, modelData.props)
                                window.selectedTool = modelData
                            }
                        }
                    }
                }
            }
        }
    }*/
}
