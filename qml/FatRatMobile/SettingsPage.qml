import QtQuick 1.0
import com.nokia.meego 1.0

Page {

    Text {
        id: text1
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        height: 40
        color: "silver"
        text: qsTr("Settings")
        font.pixelSize: 34
    }

    Text{
        id: text2
        anchors.top: text1.bottom
        anchors.topMargin: 5
        //width: parent.width
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        height: 60
        text: qsTr("Default download directory")
        font.pixelSize: 32
    }
    Text {
        id: text3
        anchors.top: text2.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 13
        width: 309
        height: 40
        text: qsTr("/documents/downloads")
        font.pixelSize: 24
    }

    Button{
        id: button1
        width: 120
        anchors.top: text3.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: "Browse"
    }

    Button{
        id: button2
        width: 120
        anchors.top: button1.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: "Proxy settings"
    }

    Text {
        id: text4
        anchors.top: button2.bottom
        anchors.topMargin: 45
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 200
        height: 80
        text: qsTr("Minimum speed: ")
        font.pixelSize: 24
    }

    TextField {
        id: text_input2
        anchors.left: text4.right
        anchors.leftMargin: 5
        anchors.topMargin: -8
        anchors.top: text4.top
        width: 140
        height: 40
        font.pixelSize: 32
    }
    Text {
        id: text5
        anchors.top: text4.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 200
        height: 80
        text: qsTr("Maximum speed: ")
        font.pixelSize: 24
    }

    TextField {
        id: text_input3
        anchors.top: text5.top
        anchors.topMargin: -8
        anchors.left: text5.right
        anchors.leftMargin: 5
        width: 140
        height: 40
        font.pixelSize: 32
    }


    tools: ToolBarLayout {
        id: pageSpecificTools
        // ToolButton to move back to the HomePage
        ToolIcon {
            anchors.right: parent.right
            anchors.rightMargin: 5;
            iconId: "toolbar-back"
            // on click signal pop the page from the stack to go back
            onClicked: appWindow.pageStack.pop();
        }
        ToolButton{
            anchors.left: parent.left
            anchors.leftMargin: 5;
            text: "Add to queue"
            onClicked: appWindow.pageStack.pop();
        }
    }

}
