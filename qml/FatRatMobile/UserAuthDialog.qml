import QtQuick 1.0
import com.nokia.meego 1.0

Dialog {
    id: userAuthDialog
    content: Column {

        spacing: 8
        anchors.left: parent.left; anchors.right: parent.right

        Label {
            id: userLabel
            text:  "User:"
            font.pixelSize: 32
            color: "white"
        }

        TextField{
            id: userField
            anchors{
                right: parent.right; left: parent.left
            }
        }

        Label {
            id : passwordLabel
            text:  "Password:"
            font.pixelSize: 32
            color: "white"
        }

        TextField{
            id: passwordField
            echoMode: TextInput.Password
            anchors{
                right: parent.right; left: parent.left
            }
        }
        CheckBox{
            id: passCheckBox
            checked: false
            onClicked: {
                if(checked){
                    passwordField.echoMode = TextInput.Normal
                }
                else{
                    passwordField.echoMode = TextInput.Password
                }
            }
        }
        Label {
            id : checkBoxLabel
            anchors.left: passCheckBox.right
            anchors.margins: 10
            anchors.top: passwordField.bottom
            anchors.topMargin: 10
            text:  "Visible password"
            font.pixelSize: 32
            color: "white"
        }

    }
    buttons: ButtonRow {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -15
        anchors { left: parent.left; right:  parent.right; margins: 15 }
        Button { text: "Ok" ; onClicked: {
                if ((userField.text != "") && (passwordField.text != "")){
                    newTransfer.authData("",userField.text,passwordField.text)
                    userAuthDialog.accept()
                }
            }
        }
        Button { text: "Cancel"; onClicked: userAuthDialog.reject() }

    }
}
