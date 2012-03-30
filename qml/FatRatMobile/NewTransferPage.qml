import QtQuick 1.0
import com.nokia.meego 1.0

Page {

    id: newTransferPage

    Row {
        id: row
        anchors.top: headerLabel.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 5
        spacing: 30
        RadioButton {
            id: radioButton_download
            text: "Download"
            checked: true
            onClicked: if (radioButton_upload.checked){
                        radioButton_upload.checked=false
                        radioButton_download.checked=true
                        newTransferPage.state = "download"
                       }
        }
        RadioButton {
            id: radioButton_upload
            checked: false
            text: "Upload"
            onClicked: if (radioButton_download.checked){
                        radioButton_upload.checked=true
                        radioButton_download.checked=false
                        newTransferPage.state = "upload"
                       }
        }
     }

     Text {
         id: sourceText
         anchors.top: row.bottom
         anchors.left: parent.left
         anchors.leftMargin: 10
         anchors.topMargin: 5
     }

     Button {
         id: addFilesSingleDialogButton
         anchors.top: row.bottom
         anchors.left: sourceText.right
         anchors.leftMargin: 10
         anchors.topMargin: 5
         text: "Add links from textfile"
         width: 280
         iconSource: "images/button_add.png"
         onClicked: {
             //addFilesSingleSelectionDialog.open();
             sourceInput.text = newTransfer.addTextFile()
         }
     }

     Rectangle {
         id: authButton
         height: 48
         width: 48
         anchors.top: row.bottom
         anchors.right: parent.right
         anchors.rightMargin: 10
         Image {
             anchors.fill: parent
             source:"images/auth.png"
         }
         MouseArea{
             anchors.fill: parent
             onClicked: userAuthDialog.open()
         }

     }

    TextArea {
        anchors.top: addFilesSingleDialogButton.bottom
        anchors.topMargin: 15
        //width: parent.width
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        height: Math.max (200, implicitHeight)
        id: sourceInput
        //text: qsTr("")
        //text: singleSelectionDialog.model.get(singleSelectionDialog.selectedIndex).name
        //text: source
        font.pixelSize: 24
    }

    Text {
        id: destinationText
        anchors.top: sourceInput.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 5
        font.pixelSize: 32
        text: "Destination: "
    }
    TextField {
        id: destinationTextInput
        anchors.top: destinationText.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 5
        width: 320
        height: 40
        text: "//home/"
        font.pixelSize: 32
    }

    Button{
        id: browseButton
        width: 120
        anchors.top: sourceInput.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: "Browse"
        onClicked: destinationTextInput.text = newTransfer.browse(destinationTextInput.text)
    }

    Text {
        id: transferType
        anchors.top: destinationTextInput.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.topMargin: 10
        height: 40
        text: "Download as: "
        font.pixelSize: 32

    }


    Button {
        id: transferDownloadSingleSelectionDialogButton
        anchors.top: destinationTextInput.bottom
        anchors.topMargin: 10
        anchors.left: transferType.right
        anchors.leftMargin: 5
        text: "Auto detect"
        width: 250
        //iconSource: "images/button_add.png"
        onClicked: {
            transferDownloadSingleSelectionDialog.open();
        }
    }

    Text {
        id: minSpeed
        anchors.top: transferType.bottom
        anchors.topMargin: 35
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: qsTr("Minimum speed: ")
        font.pixelSize: 32
    }

    TextField {
        id: minSpeedInput
        anchors.right: parent.right
        anchors.top: minSpeed.top
        width: 140
        height: 40
        font.pixelSize: 32
    }
    Text {
        id: maxSpeed
        anchors.top: minSpeed.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text: qsTr("Maximum speed: ")
        font.pixelSize: 32
    }

    TextField {
        id: maxSpeedInput
        anchors.top: maxSpeed.top
        anchors.right: parent.right
        width: 140
        height: 40
        font.pixelSize: 32
    }


    CheckBox{
        id: pauseCheckBox
        text: "Paused"
        anchors.top: maxSpeed.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        checked: false
    }

//    Button{
//        id: testButton
//        anchors.left: pauseCheckBox.right
//        anchors.top: maxSpeed.bottom
//        onClicked:                            newTransfer.createTransfer(sourceInput.text,
//                                                                         radioButton_download.checked,
//                                                                         -1,
//                                                                         destinationTextInput.text,
//                                                                         parseInt(minSpeedInput.text),
//                                                                         parseInt(maxSpeedInput.text),
//                                                                         pauseCheckBox.checked)
//    }

    UserAuthDialog{
        id: userAuthDialog
    }



    SelectionDialog {
        id: addFilesSingleSelectionDialog
        titleText: "Add Special"
        selectedIndex: -1
        model: ListModel {
            ListElement { name: "Add local files.."}
            ListElement { name: "Add contents of text file" }
        }
        onSelectedIndexChanged: {
            addFilesSingleSelectionDialog.close()
            if(selectedIndex==0){
                //sourceInput.text = newTransfer.browse2;
                test=newTransfer.browse2
            }
            if(selectedIndex==1){
                //sourceInput.text = newTransfer.browse2;
                test=newTransfer.addTextFile
                sourceInput.text = test
            }
            selectedIndex= -1;

        }
    }
    SelectionDialog {
        id: transferDownloadSingleSelectionDialog
        titleText: "Download as:"
        selectedIndex: -1
        model: ListModel {
            ListElement { name: "Auto detect"}
            ListElement { name: "HTTP(S)/FTP/SFTP download" }
        }
        onSelectedIndexChanged: {
            transferDownloadSingleSelectionDialogButton.text =
                    transferDownloadSingleSelectionDialog.model.get(transferDownloadSingleSelectionDialog.selectedIndex).name
        }
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
            onClicked: if (sourceInput.text!== "") {
                           newTransfer.createTransfer(sourceInput.text,
                                                      radioButton_download.checked,
                                                      -1,
                                                      destinationTextInput.text,
                                                      parseInt(minSpeedInput.text),
                                                      parseInt(maxSpeedInput.text),
                                                      pauseCheckBox.checked)
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
            text: "New transfer"
        }

    }

    states: [
        State {
            name: "download"
            PropertyChanges {target: sourceText; text:"URIs: " }
            PropertyChanges {target: addFilesSingleDialogButton ; text:"Add contents of text file"; onClicked: {
                    test = newTransfer.addTextFile
                    sourceInput.text = test} }
            PropertyChanges {target: transferDownloadSingleSelectionDialogButton; visible: true }
            PropertyChanges {target: transferDownloadSingleSelectionDialog; visible: true }
            PropertyChanges {target: transferType; visible: true }
            PropertyChanges {target: destinationTextInput; text: "/home/"}
            PropertyChanges {target: browseButton; visible: true }
        },
        State {
            name: "upload"
            PropertyChanges {target: destinationTextInput; text: ""; width:newTransferPage.width - 20 }
            PropertyChanges {target: sourceText; text:"Files: " }
            PropertyChanges {target: addFilesSingleDialogButton; text:"Add "; onClicked: {sourceInput.text = newTransfer.browse2} }
            PropertyChanges {target: transferType; visible: false }
            PropertyChanges {target: transferDownloadSingleSelectionDialogButton; visible: false }
            PropertyChanges {target: transferDownloadSingleSelectionDialog; visible: false }
            PropertyChanges {target: browseButton; visible: false }
        }



    ]

}
