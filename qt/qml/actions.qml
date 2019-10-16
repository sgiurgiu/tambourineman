pragma Singleton

import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property alias newProject: newProjectAction;
    property alias openProject: openProjectAction;
    property alias quitApplication: quitApplicationAction;

    Action {
        id: newProjectAction
        text: qsTr("&New")
        shortcut: StandardKey.New
        icon.name:"file-new"

    }
    Action {
        id: openProjectAction
        text: qsTr("&Open")
        shortcut: StandardKey.Open
        icon.name:"file-open"

    }
    Action {
        id: quitApplicationAction
        text: qsTr("&Quit")
        shortcut: StandardKey.Quit
        icon.name: "file-quit"
        onTriggered: Qt.quit()
    }

}
