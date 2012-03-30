import QtQuick 1.0
import com.nokia.meego 1.0

Page {

    id: proxyPage
//    Text {
//        id: header
//        anchors.top: parent.top
//        anchors.topMargin: 22
//        anchors.right: parent.right
//        anchors.left: parent.left
//        anchors.leftMargin: 10
//        anchors.rightMargin: 10
//        height: 40
//        text: qsTr("Proxy  server")
//        font.pixelSize: 34
//    }

     Text {
         id: nameText
         anchors.top: headerLabel.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 40
         font.pixelSize: 24
         text: "Proxy name: "
     }
     TextField {
         id: nameTextEdit
         anchors.top: headerLabel.bottom
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.topMargin: 30
     }
     Text {
         id: ipText
         anchors.top: nameText.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 40
         font.pixelSize: 24
         text: "Proxy IP: "
     }
     TextField {
         id: ipTextEdit
         anchors.top: nameText.bottom
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.topMargin: 30
     }
     Text {
         id: portText
         anchors.top: ipText.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 40
         font.pixelSize: 24
         text: "Proxy port: "
     }
     TextField {
         id: portTextEdit
         anchors.top: ipText.bottom
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.topMargin: 30
         text: "80"
     }
     Text {
         id: userText
         anchors.top: portText.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 40
         font.pixelSize: 24
         text: "Proxy user: "
     }
     TextField {
         id: userTextEdit
         anchors.top: portText.bottom
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.topMargin: 30
     }
     Text {
         id: passwordText
         anchors.top: userText.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 40
         font.pixelSize: 24
         text: "Proxy password: "
     }
     TextField {
         id: passwordTextEdit
         anchors.top: userText.bottom
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.topMargin: 30
         echoMode: TextInput.Password
     }

    Text {
        id: proxyType
        anchors.top: passwordText.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 40
        font.pixelSize: 24
        text: "Proxy type: "

    }


    Button {
        id: proxyTypeSingleSelectionDialogButton
        anchors.top: passwordText.bottom
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 35
        text: "HTTP"
        width: 250
        onClicked: {
            proxyTypeSingleSelectionDialog.open();
        }
    }


    SelectionDialog {
        id: proxyTypeSingleSelectionDialog
        titleText: "Download as:"
        selectedIndex: 0
        model: ListModel {
            ListElement { name: "HTTP"}
            ListElement { name: "SOCKS 5" }
        }
        onSelectedIndexChanged: {
            proxyTypeSingleSelectionDialogButton.text =
                    proxyTypeSingleSelectionDialog.model.get(proxyTypeSingleSelectionDialog.selectedIndex).name
        }
    }


    tools: ToolBarLayout {
        id: pageSpecificTools
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
            text: "Save"
            onClicked: if ((nameTextEdit!=="")&&(ipTextEdit!=="")&&(portTextEdit!=="")){

                       }
        }
    }

    Rectangle {
        id: headerLabel
        width: parent ? parent.width : 480;
        height: 80
        color: "silver"
        Image {
            id: icon
            width: 80

            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                margins: 10
            }

            smooth: true
            source: "images/FatRatMobile80.png"
            fillMode: Image.PreserveAspectFit
        }
        Text {
            anchors {
                left: icon.right
                right: parent.right
                verticalCenter: parent.verticalCenter
                leftMargin: 10
                rightMargin: 10
            }
            color: "white"
            elide: Text.ElideRight

            font {
                family: platformLabelStyle.fontFamily
                pixelSize: 32
            }
            text: "Proxy server"
        }

    }

}
