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
            id: titleField
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
            id: passField
            echoMode: TextInput.Password
            anchors{
                right: parent.right; left: parent.left
            }
        }
    }
    buttons: ButtonRow {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -15
        anchors { left: parent.left; right:  parent.right; margins: 15 }
        Button { text: "Ok" ; onClicked: userAuthDialog.accept()}
        Button { text: "Cancel"; onClicked: userAuthDialog.reject() }
    }

}
