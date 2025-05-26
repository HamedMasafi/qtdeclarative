// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Window
import QtQuick.Controls 6.7
import QtQuick.Layouts
// import DockTestApp3

ApplicationWindow {
   id: window
   visible: true
   width: 640
   height: 480
   title: qsTr("Dock example")

   property var selectedTool: null

   menuBar: MenuBar {
      Menu {
         title: "File"
         MenuItem {
            text: "New"
            onClicked: documents.create()
         }
      }
      Menu {
         title: "Windows"
         Repeater {
            model: dockContainer.dockPanels
            MenuItem {
               text: modelData.title
               checkable: true
               checked: modelData.isOpen
               font.bold: modelData.isActive
               onClicked: {
                  if (modelData.isOpen)
                     modelData.close()
                  else
                     modelData.open()
               }
            }
         }
      }
   }

   DockContainer {
      id: dockContainer
      anchors.fill: parent
      popupType: DockContainer.Item

      DockLayout {
         orientation: Qt.Horizontal
         DockGroup {
            ToolsPanel {
               title: "Tools"
               canvas: documents.canvas
               icon.source: "qrc:/images/tool.png"
            }
         }
         DocumentsPanel {
            id: documents
         }

         DockGroup {
            PropertiesPanel {
               DockContainer.edge: Qt.RightEdge
               selectedItem: documents.canvas.activeItem
               icon.source: "qrc:/images/settings.png"
            }

            ExportDockPanel {
               DockContainer.edge: Qt.RightEdge
               icon.source: "qrc:/images/export.png"
            }

            UndoPanel {
               DockContainer.edge: Qt.RightEdge
               icon.source: "qrc:/images/undo.png"
            }

            OutputDockPanel {
               DockContainer.edge: Qt.BottomEdge
               icon.source: "qrc:/images/doc.png"
            }
         }
      }
   }
}
