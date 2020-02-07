import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import com.zergiu.tambourineman 1.0 as TM
import com.zergiu.tambourineman.proto 1.0

ApplicationWindow
{
    visible: true
    width: 800
    height: 640
    title: qsTr("Tambourine Man")
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    SystemPalette { id: standardPalette; colorGroup: SystemPalette.Active }
    SystemPalette { id: disabledPalette; colorGroup: SystemPalette.Disabled }

    Component.onCompleted: {
        console.log("completed ")
    }

    menuBar: MenuBar {
        Menu {
           title: "&File"
           MenuItem {
               action:  TM.Actions.newProject
           }
           MenuItem {
               action: TM.Actions.openProject
           }
           MenuSeparator{}
           MenuItem {
               action: TM.Actions.quitApplication
           }
        }
    }

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            anchors.left: parent
            Button {
                text: "Open Proto"
                onClicked: protoFileLoaderModel.loadFile("asdads")
            }
            ListView {
                width: 180
                height: 500
                orientation: Qt.Vertical
                model: contactModel
                delegate: contactDelegate
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                focus: true
            }

        }

        ColumnLayout {
            anchors.centerIn: parent
            Rectangle {
                anchors.fill: parent
                border.color: black
                border.width: 2
            }
        }

        ColumnLayout {
            anchors.right: parent
            Rectangle {
                anchors.fill: parent
                border.color: red
                border.width: 2
            }
        }
    }

    ListModel {
        id: contactModel
        ListElement {
               name: "Bill Smith"
               number: "555 3264"
           }
           ListElement {
               name: "John Brown"
               number: "555 8426"
           }
           ListElement {
               name: "Sam Wise"
               number: "555 0473"
           }
    }

    Component {
        id: contactDelegate
        Item {
            width: 180; height: 40
            Text {
                    text: name + ": " + number
            }
        }
    }

    ProtoFileLoaderModel {
        id: protoFileLoaderModel
    }


    footer :  ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: "Status1"
            }
            ToolSeparator {}
            Label {
                Layout.fillWidth: true
                text: "things"
            }
            ToolSeparator {}
            Label {
                text: "Others"
            }
        }
    }

}
