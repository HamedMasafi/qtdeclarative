import QtQuick
import QtQuick.Controls

DockContainer {
    width: 640
    height: 480

    DockLayout {
        orientation: Qt.Horizontal

        DockGroup {
            DockWidget {
            title: "Fonts"
            contentItem: ListView {
                id: listView
                width: 160
                height: 440

                model: Qt.fontFamilies()

                delegate: ItemDelegate {
                    text: modelData
                    width: listView.width
                    onClicked: textEditor.font.family = modelData

                    required property string modelData
                }

                ScrollIndicator.vertical: ScrollIndicator { }
            }
        }
        Item {
            id: mainArea
        }
        DockGroup {
            DockWidget {
                title: "Editor"
                contentItem: TextArea {
                    id: textEditor
                    x: 180
                    width: 480
                    height: 440
                    text: "Text to edit, as usual. Perhaps you can change the font."
                }
            }
        }
    }
}
