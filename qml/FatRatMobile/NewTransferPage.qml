import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    property string source
    id: newTransferPage
    orientationLock: PageOrientation.LockPortrait

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


        Rectangle {
            id: authButton
            height: 48
            width: 48
            anchors.top: row.bottom
            //   anchors.topMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 10
            color: "transparent"
            Image {
                anchors.fill: parent
                source:"images/auth.png"
            }
            MouseArea{
                anchors.fill: parent
                onClicked: userAuthDialog.open()
            }

        }

        Label {
            id: sourceText
            anchors.top: row.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 20
            font.pixelSize: 24
        }

        TextArea {
            anchors.top: sourceText.bottom
            //   anchors.topMargin: 10
            //width: parent.width
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            height: Math.max (200, implicitHeight)
            id: sourceInput
            font.pixelSize: 24
        }

        Button {
            id: addFilesSingleDialogButton
            anchors.top: sourceInput.bottom
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.topMargin: 5
            text: "Add links from text file"
            width: 350
            iconSource: "images/button_add.png"
        }

        Label {
            id: destinationText
            anchors.top: addFilesSingleDialogButton.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 5
            font.pixelSize: 24
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
            text: "/home/user/MyDocs/Downloads"
            font.pixelSize: 24
        }

        Button{
            id: browseButton
            width: 120
            anchors.top: destinationText.bottom
            //  anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            text: "Browse"
            // onClicked: destinationTextInput.text = newTransfer.browse(destinationTextInput.text)
            onClicked:  directorySelector.open()
        }

        Label {
            id: transferType
            anchors.top: destinationTextInput.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.topMargin: 20
            height: 40
            text: "Download as: "
            font.pixelSize: 24

        }


        Button {
            id: transferDownloadSingleSelectionDialogButton
            anchors.top: destinationTextInput.bottom
            anchors.topMargin: 10
            anchors.left: transferType.right
            anchors.leftMargin: 5
            text: "Auto detect"
            width: 250
            onClicked: {
                transferDownloadSingleSelectionDialog.open();
            }
        }

        Label {
            id: minSpeed
            anchors.top: transferType.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: qsTr("Down speed limit (kB/s): ")
            font.pixelSize: 24
        }

        TextField {
            id: minSpeedInput
            anchors.right: parent.right
            anchors.top: minSpeed.top
            width: 140
            height: 40
            font.pixelSize: 24
        }
        Label {
            id: maxSpeed
            anchors.top: minSpeed.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            text: qsTr("Up speed limit (kB/s):  ")
            font.pixelSize: 24
        }

        TextField {
            id: maxSpeedInput
            anchors.top: maxSpeed.top
            anchors.right: parent.right
            width: 140
            height: 40
            font.pixelSize: 24
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


        UserAuthDialog{
            id: userAuthDialog
        }

        FileSelector {
            id: fileSelector
            onAccepted: {
                source = selectedFile
                if (source!=""){
                    if (sourceInput.text != ""){
                        sourceInput.text = sourceInput.text + "\n" +source
                    }
                    else {
                        sourceInput.text = source
                    }
                }
            }
        }
        DirectorySelector {
            id: directorySelector
            onAccepted: {
                if (folderModel.folder != ""){
                    destinationTextInput.text = newTransfer.removeFileFromString(folderModel.folder)
                }
            }

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
                ListElement { name: "HTTP(S)/FTP download" }
            }
            onSelectedIndexChanged: {
                transferDownloadSingleSelectionDialogButton.text =
                        transferDownloadSingleSelectionDialog.model.get(transferDownloadSingleSelectionDialog.selectedIndex).name
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
                                                      (parseInt(minSpeedInput.text)*1024),
                                                      (parseInt(maxSpeedInput.text)*1024),
                                                      pauseCheckBox.checked)
                           itemList.refresh()
                           appWindow.pageStack.pop()
                       }
        }
    }
    states: [
        State {
            name: "download"
            PropertyChanges {target: sourceText; text:"Files to download: " }
            PropertyChanges {target: addFilesSingleDialogButton ; text:"Add links from text file"; onClicked: {
                    fileSelector.open()}}
            PropertyChanges {target: transferDownloadSingleSelectionDialogButton; visible: true }
            PropertyChanges {target: transferDownloadSingleSelectionDialog; visible: true }
            PropertyChanges {target: transferType; visible: true }
            PropertyChanges {target: destinationTextInput; text: "/home/user/MyDocs/Downloads"}
            PropertyChanges {target: browseButton; visible: true }
            PropertyChanges {target: fileSelector;titleText: "Choose text file" ;onAccepted: {
                    if (selectedFile!=""){
                        if (sourceInput.text != ""){
                            sourceInput.text = sourceInput.text + "\n" + newTransfer.getLinksFromFile(selectedFile)
                        }
                        else {
                            sourceInput.text = newTransfer.getLinksFromFile(selectedFile)
                        }
                    }

                }
                filterList: [ "*.txt" ]
            }
        },
        State {
            name: "upload"
            PropertyChanges {target: destinationTextInput; text: ""; width:newTransferPage.width - 20 }
            PropertyChanges {target: sourceText; text:"Files to upload: " }
            PropertyChanges {target: addFilesSingleDialogButton; text:"Add files to upload "; onClicked: {
                    fileSelector.open()}}
            PropertyChanges {target: transferType; visible: false }
            PropertyChanges {target: transferDownloadSingleSelectionDialogButton; visible: false }
            PropertyChanges {target: transferDownloadSingleSelectionDialog; visible: false }
            PropertyChanges {target: browseButton; visible: false }
            PropertyChanges {target: fileSelector; titleText:"Choose file" ;onAccepted: {
                    if (selectedFile!=""){
                        if (sourceInput.text != ""){
                            sourceInput.text = sourceInput.text + "\n" + newTransfer.removeFileFromString(selectedFile)
                        }
                        else {
                            sourceInput.text = newTransfer.removeFileFromString(selectedFile)
                        }
                    }
                }
                filterList: [ "*" ] }
        }



    ]
}
