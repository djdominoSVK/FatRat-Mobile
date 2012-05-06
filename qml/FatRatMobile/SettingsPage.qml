import QtQuick 1.0
import com.nokia.meego 1.0

Page {

    id: settingsPage
    orientationLock: PageOrientation.LockPortrait


     Label {
         id: refreshText
         anchors.top: parent.top
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 100
         font.pixelSize: 24
         text: "Refresh GUI every "
     }
     TextField {
         id: refreshTextEdit
         anchors.top: parent.top
         anchors.right: secondsText.left
         anchors.rightMargin: 10
         anchors.topMargin: 88
         font.pixelSize: 24
         width: 80
         text: settingsMethods.getRefresh()
     }
     Label {
         id: secondsText
         anchors.top: parent.top
         anchors.right: parent.right
         anchors.rightMargin: 10
         anchors.topMargin: 100
         font.pixelSize: 24
         text: "seconds "
     }

     Label {
         id: maximumTransfersLabel
         anchors.top: refreshText.bottom
         anchors.topMargin: 40
         anchors.left: parent.left
         anchors.leftMargin: 10
         text: "Maximum active transfers: "

     }
     Label {
         id: maximumTransfersValueLabel
         anchors.top: refreshText.bottom
         anchors.topMargin: 40
         anchors.left: maximumTransfersLabel.right
         anchors.leftMargin: 10
         text: (settingsMethods.getNetworkMaximum() === -1) ? "unlimited " : maximumTransfersLabelTextEdit.text
     }
     CheckBox{
         id: limitedTransfersCheckBox
         anchors.top: maximumTransfersLabel.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 14
         checked : !(settingsMethods.getNetworkMaximum() === -1)
         onClicked: {
             if(checked){
                 maximumTransfersLabelTextEdit.visible= true
                 maximumTransfersValueLabel.text= maximumTransfersLabelTextEdit.text
             }
             else{
                maximumTransfersLabelTextEdit.visible= false
                 maximumTransfersValueLabel.text = "unlimited "
             }
         }
     }
     Label {
         id : checkBoxLabel
         anchors.top: maximumTransfersLabel.bottom
         anchors.left: limitedTransfersCheckBox.right
         anchors.leftMargin: 10
         anchors.topMargin:17
         text:  "Limited"
         font.pixelSize: 24
     }
     TextField {
         id: maximumTransfersLabelTextEdit
         anchors.top: maximumTransfersLabel.bottom
         anchors.left: checkBoxLabel.right
         anchors.leftMargin: 40
         anchors.topMargin: 7
         width: 100
         visible: !(settingsMethods.getNetworkMaximum() === -1)
         text: settingsMethods.getNetworkMaximum()
         onTextChanged: if (limitedTransfersCheckBox.checked) maximumTransfersValueLabel.text= maximumTransfersLabelTextEdit.text
     }

     Label {
         id: globalLabel
         anchors.top: limitedTransfersCheckBox.bottom
         anchors.topMargin: 30
         anchors.left: parent.left
         anchors.leftMargin: 10
         text: "Global speed limits"
         font.pixelSize: 32
     }

     Label {
         id: downSpeed
         anchors.top: globalLabel.bottom
         anchors.topMargin: 10
         anchors.left: parent.left
         anchors.leftMargin: 10
         text: "Down speed limit (kB/s): "
         font.pixelSize: 24
     }
     TextField {
         id: downSpeedInput
         anchors.right: parent.right
         anchors.top: downSpeed.top
         anchors.rightMargin: 10
         anchors.topMargin: -8
         width: 140
         height: 40
         font.pixelSize: 24
         text: settingsMethods.getNetworkSpeedDown()
     }
     Label {
         id: upSpeed
         anchors.top: downSpeed.bottom
         anchors.topMargin: 10
         anchors.left: parent.left
         anchors.leftMargin: 10
         text: "Up speed limit (kB/s):  "
         font.pixelSize: 24
     }

     TextField {
         id: upSpeedInput
         anchors.top: upSpeed.top
         anchors.right: parent.right
         anchors.rightMargin: 10
         anchors.topMargin: -8
         width: 140
         height: 40
         font.pixelSize: 24
         text: settingsMethods.getNetworkSpeedUp()
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
            property string numTransfers
            anchors.left: parent.left
            anchors.leftMargin: 5;
            text: "Save"
            onClicked: {
                if (!limitedTransfersCheckBox.checked){
                        numTransfers = "-1"}
                else{
                    numTransfers = maximumTransfersLabelTextEdit.text
                }

                settingsMethods.saveSettings(refreshTextEdit.text,downSpeedInput.text, upSpeedInput.text, numTransfers);
                settingsMethods.initSettings()
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
            text: "Settings"
        }

    }


}
