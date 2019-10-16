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

//    header: ToolBar {
//        RowLayout {
//            anchors.fill: parent
//            spacing: 10

//             ToolButton {
//                 action: TM.Actions.newProject
//             }
//             Label {
//                 Layout.fillWidth: true
//                 text: ""
//             }

//             ToolButton {
//                 action: TM.Actions.quitApplication
//             }
//        }
//    }



    RowLayout {
        anchors.fill: parent
        Button {
            text: "Open Proto"
            onClicked: protoFileLoader.loadFile("asdads")
        }
    }

    ProtoFileLoader {
        id: protoFileLoader
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
