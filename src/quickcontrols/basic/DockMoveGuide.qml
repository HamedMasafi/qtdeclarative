// Copyright (C) 2025 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.DockMoveGuide {
    id: control
    implicitHeight: 300
    implicitWidth: 250

/*    Rectangle {
        x: control.highlightRect.x
        y: control.highlightRect.y
        width: control.highlightRect.width
        height: control.highlightRect.height

        opacity: 0.4
        visible: control.currentSide !== DockMoveGuide.None

        // Behavior on x { NumberAnimation{duration: 200} }
        // Behavior on y { NumberAnimation{duration: 200} }
        // Behavior on width { NumberAnimation{duration: 200} }
        // Behavior on height { NumberAnimation{duration: 200} }

        color: control.palette.highlight
        border.color: control.palette.highlightedText
    }
    */
    panelCenter: Rectangle {
        implicitHeight: 30
        implicitWidth: 30

        height: 30
        width: 30
        Image {
            width: parent.width
            height: parent.height
            rotation: 180
            source: control.currentSide === DockMoveGuide.Fill
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-fill-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-fill-indicator.png"
        }
    }
    panelLeft: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 180
            source: control.currentSide === DockMoveGuide.PanelLeft
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    panelRight: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            source: control.currentSide == DockMoveGuide.PanelRight
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    panelTop: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 270
            source: control.currentSide == DockMoveGuide.PanelTop
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    panelBottom: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 90
            source: control.currentSide == DockMoveGuide.PanelBottom
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }

    windowLeft: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 180
            source: control.currentSide == DockMoveGuide.WindowLeft
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    windowRight: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            source: control.currentSide == DockMoveGuide.WindowRight
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    windowTop: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 270
            source: control.currentSide == DockMoveGuide.WindowTop
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
    windowBottom: Rectangle {
        implicitHeight: 26
        implicitWidth: 26
        Image {
            width: parent.width
            height: parent.height
            rotation: 90
            source: control.currentSide == DockMoveGuide.WindowBottom
                ? "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator-active.png"
                : "qrc:/qt-project.org/imports/QtQuick/Controls/Basic/images/dock-drop-indicator.png"
        }
    }
}
