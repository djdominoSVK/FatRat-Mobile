import QtQuick 1.0
import com.nokia.meego 1.0

Page {

    id: proxyPage
    orientationLock: PageOrientation.LockPortrait
    state: settingsMethods.isProxyEnabled() ? "enabled" : "disabled"
    CheckBox{
        id: enabledCheckBox
        anchors.top: headerLabel.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 40
        checked: settingsMethods.isProxyEnabled()
        onClicked: {
            if(checked){
                parent.state= "enabled"
            }
            else{
                parent.state= "disabled"
            }
        }
    }
    Label {
        id : checkBoxLabel
        anchors.top: headerLabel.bottom
        anchors.left: enabledCheckBox.right
        anchors.leftMargin: 10
        anchors.topMargin: 43
        text:  "Enable proxy"
        font.pixelSize: 24
    }

    Item {
        id: proxyField
        anchors.fill: parent
     Text {
         id: nameText
         anchors.top: parent.top
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 200
         font.pixelSize: 24
         text: "Proxy name: "
     }
     TextField {
         id: nameTextEdit
         anchors.bottom: ipTextEdit.top
         anchors.right: parent.right
         anchors.rightMargin: 35
         anchors.bottomMargin: 20
         text: settingsMethods.getProxyName()
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
         text: settingsMethods.getIp()
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
         text: (settingsMethods.getPort() === "") ? "80" : settingsMethods.getPort()
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
         text: settingsMethods.getUser()
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
         text: settingsMethods.getPassword()
     }

     CheckBox{
         id: passCheckBox
         anchors.top: passwordTextEdit.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 30
         checked: false
         onClicked: {
             if(checked){
                 passwordTextEdit.echoMode = TextInput.Normal
             }
             else{
                 passwordTextEdit.echoMode = TextInput.Password
             }
         }
     }
     Label {
         id : passCheckBoxLabel
         anchors.left: passCheckBox.right
         anchors.margins: 10
         anchors.top: passwordTextEdit.bottom
         anchors.topMargin: 35
         text:  "Visible password"
         font.pixelSize: 24
     }


    Text {
        id: proxyType
        anchors.top: passCheckBox.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 40
        font.pixelSize: 24
        text: "Proxy type: "

    }


    Button {
        id: proxyTypeSingleSelectionDialogButton
        anchors.top: passCheckBox.bottom
        anchors.topMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 35
        text: (ett.getType() === 0) ? "HTTP" : "SOCKS 5"
        width: 250
        onClicked: {
            proxyTypeSingleSelectionDialog.open();
        }
    }
    //    Button{
    //        id: test
    //        anchors.top: proxyTypeSingleSelectionDialogButton.bottom
    //        anchors.topMargin: 30
    //        anchors.right: parent.right
    //        anchors.rightMargin: 35
    //        text: "Save"
    //        onClicked: {
    //                       newTransfer.saveProxy(proxyTypeSingleSelectionDialog.selectedIndex,nameTextEdit.text,
    //                                             ipTextEdit.text,portTextEdit.text,userTextEdit.text,passwordTextEdit.text,
    //                                             enabledCheckBox.checked)
    //                   }
    //    }
    //    Button {
    //        anchors.top: test.bottom
    //        anchors.topMargin: 30
    //        anchors.right: parent.right
    //        anchors.rightMargin: 35
    //       //iconId: "toolbar-back"
    //        // on click signal pop the page from the stack to go back
    //        onClicked: appWindow.pageStack.pop();
    //    }
    }


    SelectionDialog {
        id: proxyTypeSingleSelectionDialog
        titleText: "Download as:"
        selectedIndex: settingsMethods.getType()
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
            onClicked: appWindow.pageStack.pop();
        }
        ToolButton{
            anchors.left: parent.left
            anchors.leftMargin: 5;
            text: "Save"
            onClicked: { settingsMethods.saveProxy(proxyTypeSingleSelectionDialog.selectedIndex,nameTextEdit.text,
                                                 ipTextEdit.text,portTextEdit.text,userTextEdit.text,passwordTextEdit.text,
                                                 enabledCheckBox.checked)
                         appWindow.pageStack.pop()
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
                //family: platformLabelStyle.fontFamily
                pixelSize: 32
            }
            text: "Proxy server"
        }

    }

    states: [
        State {name: "disabled"
//            PropertyChanges {target: nameTextEdit; visible: false }
//            PropertyChanges {target: ipTextEdit; visible: false }
//            PropertyChanges {target: portTextEdit; visible: false }
            PropertyChanges {target: proxyField; visible: false }
//            PropertyChanges {target: userTextEdit; visible: false }
//            PropertyChanges {target: passwordTextEdit; visible: false }
//            PropertyChanges {target: proxyTypeSingleSelectionDialogButton; visible: false }

        },
        State {name: "enabled"
//            PropertyChanges {target: nameTextEdit; visible: true }
//            PropertyChanges {target: ipTextEdit; visible: false }
//            PropertyChanges {target: portTextEdit; visible: false }
//            PropertyChanges {target: userTextEdit; visible: false }
//            PropertyChanges {target: passwordTextEdit; visible: false }
//            PropertyChanges {target: proxyTypeSingleSelectionDialogButton; visible: false }
            PropertyChanges {target: proxyField; visible: true }
        }

    ]

}
