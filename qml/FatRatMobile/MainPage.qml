import QtQuick 1.0
import com.nokia.meego 1.0


Page {
    id : listViewPage
    tools: commonTools
    orientationLock: PageOrientation.LockPortrait

    function deleteWithData(index)
    {if ((index>-1)&&(listTransfers.count>index)){
            queue.removeWithData(index,false)
        }
        listViewPage.state = "active"
    }

    function deleteWithoutData(index)
    {if ((index>-1)&&(listTransfers.count>index)){
            queue.remove(index,false)

        }
        listViewPage.state = "active"
    }


    Component {
        id: transferDelegate
        Item {
            id: transfer

            property real detailsOpacity : 0

            width: listTransfers.width
            height: 80

            MouseArea {
                anchors.top: parent.top
                anchors.bottom: parent.top
                anchors.bottomMargin: -80
                anchors.right: parent.right
                anchors.left: parent.left
                onClicked: {
                    listTransfers.currentIndex = index
                    transfer.state = 'details'
                    listViewPage.state = "details"
                }
                onPressAndHold: {
                    listTransfers.currentIndex = index
                    transferDialog.name = name
                    transferDialog.destination = destination
                    transferDialog.open()
                }

            }

            Image {
                id: iconImage
                width: 42
                height: 42
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 15
                source: ("images" + model.state)
            }

            Text {
                id: filenameTxt
                font.family: "Tahoma"
                color: "black"
                font.pixelSize: 24
                elide: Text.ElideRight
                text: name
                anchors.left: iconImage.right
                anchors.leftMargin: 5
                anchors.right: speedTxt.left
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 20
                visible: true
            }

            ProgressBar {
                id: progressBar
                value: fProgress
                maximumValue: 100
                minimumValue: 0
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: iconImage.bottom
                anchors.topMargin: 4
            }

            Text {
                id: speedTxt
                font.family: "Tahoma"
                color: "black"
                font.pixelSize: 24
                text: if(speed !== "") { return speed } else { return "n/a" }
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 20
            }

            Item {
                id: details
                anchors { top: parent.top; topMargin: 10; bottom: parent.bottom; bottomMargin: 10 }
                width: listTransfers.width
                opacity: transfer.detailsOpacity


                Label {
                    id: progressLabel
                    anchors.top: parent.top
                    anchors.topMargin: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    font.bold: true
                    text: "Progress: "
                }
                Text {
                    id: transferProgress
                    anchors.top: parent.top
                    anchors.topMargin: 90
                    anchors.left: progressLabel.right
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    elide: Text.ElideRight
                    text: progress
                }
                Label {
                    id: sourceLabel
                    anchors.top: transferProgress.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10

                    font.pixelSize: 24
                    font.bold: true
                    text: "Source: "
                }
                Text {
                    id: transferSource
                    anchors.top: sourceLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    font.pixelSize: 24
                    elide: Text.ElideMiddle
                    text: source
                    MouseArea{
                        anchors.fill: parent
                        onPressAndHold: {
                            urlTransferDialog.url= transferSource.text
                            urlTransferDialog.open()
                        }
                    }

                }

                Label {
                    id: destinationLabel
                    anchors.top: transferSource.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    font.bold: true
                    text: "Destination: "
                }
                Text {
                    id: transferDestinatipon
                    anchors.top: destinationLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    font.pixelSize: 24
                    elide: Text.ElideMiddle
                    text: destination
                    MouseArea{
                        anchors.fill: parent
                        onPressAndHold: {
                            urlTransferDialog.url= transferDestinatipon.text
                            urlTransferDialog.open()
                        }
                    }
                }

                Label {
                    id: timeLeftLabel
                    anchors.top: transferDestinatipon.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    font.bold: true
                    text: "Time left: "
                }
                Text {
                    id: transferTimeLeft
                    anchors.top: transferDestinatipon.bottom
                    anchors.topMargin: 10
                    anchors.left: timeLeftLabel.right
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    text: timeLeft
                }
                Label {
                    id: sizeLabel
                    anchors.top: timeLeftLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    font.bold: true
                    text: "Size: "
                }
                Text {
                    id: transferSize
                    anchors.top: transferTimeLeft.bottom
                    anchors.topMargin: 10
                    anchors.left: sizeLabel.right
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    text: size
                }
                Label {
                    id: messageLabel
                    anchors.top: sizeLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    font.bold: true
                    elide: Text.ElideRight
                    text: "Message: "
                }
                Text {
                    id: transferMessage
                    anchors.top: messageLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10

                    font.pixelSize: 24
                    text: message
                }

            }


            states: State {
                name: "details"
                PropertyChanges {target: iconBack; onClicked:{
                        transfer.state = ''
                        listViewPage.state = "active"
                        listTransfers.currentIndex = -1}}
                PropertyChanges { target: transfer; detailsOpacity: 1; x: 0 }
                PropertyChanges { target: transfer; height:  listTransfers.height } // Fill the entire list area with the detailed view
                PropertyChanges { target: transfer.ListView.view; explicit: true; contentY: transfer.y }
                PropertyChanges { target: moveUp; visible: false}
                PropertyChanges { target: moveDown; visible: false}
                PropertyChanges { target: moveTop; visible: false}
                PropertyChanges { target: moveBottom; visible: false}
                PropertyChanges { target: transfer.ListView.view; interactive: false }
            }

            transitions: Transition {
                ParallelAnimation {
                    ColorAnimation { property: "color"; duration: 500 }
                    NumberAnimation { duration: 300; properties: "detailsOpacity,x,contentY,height,width" }
                }
            }
        }
    }

    ListView {
        id: listTransfers
        width:parent.width
        height: parent.height
        currentIndex: -1

        anchors.top: headerLabel.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        clip: true
        model: itemList
        delegate: transferDelegate



        onCurrentIndexChanged: {console.log("index "+ listTransfers.currentIndex)}
        onCurrentItemChanged: console.log("item")
        Timer {
            id: refresh
            interval: 1000;
            running: true;
            repeat: true
            onTriggered:  {
                itemList.refresh()
                downloadText.text = "Download: " + queue.getDownSpeed()
                uploadText.text = "Upload: " + queue.getUpSpeed()
                refresh.interval = settingsMethods.getRefresh() * 1000

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
            id:title
            anchors {
                left: icon.right
                verticalCenter: parent.verticalCenter
                leftMargin: 10
            }
            color: "white"
            elide: Text.ElideRight

            font.pixelSize: 32
            text: "FatRat"
        }
        Text {
            id: downloadText
            anchors {
                right: parent.right
                rightMargin: 10
                top: parent.top
                topMargin: 10
            }
            color: "white"
            //elide: Text.ElideRight

            font.pixelSize: 24
            text: "Download: 0.0 kB/s"
        }
        Text {
            id:uploadText
            anchors {
                right: parent.right
                rightMargin: 10
                bottom: parent.bottom
                bottomMargin: 10
            }
            color: "white"
            elide: Text.ElideRight

            font.pixelSize: 24
            text: "Upload: 0.0 kB/s"
        }

    }
    Dialog {
        id: deleteTransferDialog
        content:Item {
            id: deleteName
            height: 50
            width: parent.width
            Text {
                id: deleteText
                font.pixelSize: 22
                anchors.centerIn: parent
                color: "white"
                text: "Really remove this transfer?"
            }
        }

        buttons:  ButtonColumn {
            id:buttonsDelete
            spacing: 15
            style: ButtonStyle { }
            anchors.horizontalCenter: parent.horizontalCenter
            Button {text: "Delete with data"; onClicked:
                {   deleteWithData(listTransfers.currentIndex)
                    listTransfers.currentItem.state = ''
                    deleteTransferDialog.accept()
                }
            }
            Button { text: "Delete without data"; onClicked:
                {
                    deleteWithoutData(listTransfers.currentIndex);
                    listTransfers.currentItem.state = ''
                    deleteTransferDialog.accept()
                }
            }
            Button {text: "Cancel"; onClicked: deleteTransferDialog.close()}
        }
    }
    Dialog {
        property string url
        id: urlTransferDialog
        content:Item {
            id: urlName
            height: 300
            width: parent.width

            Text {
                id: urlText
                font.pixelSize: 22
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "white"
                text: "Source: "
            }
            TextArea{
                anchors.top: urlText.bottom
                width: parent.width
                height: 250
                text: urlTransferDialog.url
            }
        }

        buttons:  ButtonColumn {
            spacing: 5
            style: ButtonStyle { }
            anchors.horizontalCenter: parent.horizontalCenter
            Button {text: "Cancel"; onClicked: urlTransferDialog.close()}
        }
    }
    Dialog {
        property string name
        property string destination

        id: transferDialog
        content:Item {
            id: name
            height: 50
            width: parent.width
            Text {
                id: text
                font.pixelSize: 22
                anchors.centerIn: parent
                color: "white"
                text: "What do you want to do?"
            }
        }

        buttons:  ButtonColumn {
            spacing: 15
            style: ButtonStyle { }
            anchors.horizontalCenter: parent.horizontalCenter
            Button {text: "Open file";
                onClicked:
                {   Qt.openUrlExternally(transferDialog.destination + "/" + transferDialog.name);
                    transferDialog.accept()
                    listTransfers.currentIndex = -1
                }
            }
            Button {
                id: moveUp
                text: "Move up"
                onClicked:
                {
                    queue.moveUp(listTransfers.currentIndex)
                    transferDialog.accept()
                    listTransfers.currentIndex = -1
                }
              }
            Button {
                id: moveDown
                text: "Move down"
                onClicked:
                {
                    queue.moveDown(listTransfers.currentIndex)
                    transferDialog.accept()
                    listTransfers.currentIndex = -1
                }
            }
            Button {
                id: moveTop
                text: "Move to top"
                onClicked:
                {
                    queue.moveToTop(listTransfers.currentIndex)
                    transferDialog.accept()
                    listTransfers.currentIndex = -1
                }
            }
            Button {
                id:moveBottom
                text: "Move to bottom"
                onClicked:
                {
                    queue.moveToBottom(listTransfers.currentIndex)
                    transferDialog.accept()
                    listTransfers.currentIndex = -1
                }
            }
            Button {text: "Cancel"; onClicked: transferDialog.close()}
        }
    }


    ToolBarLayout {
        id: commonTools
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        }
        visible: true

        ToolIcon {
            id: iconDelete
            iconId: "toolbar-delete"
            visible: false
            onClicked: deleteTransferDialog.open()
        }
        ToolIcon {
            id: iconAdd
            iconId: "toolbar-add"
            onClicked: pageStack.push(Qt.resolvedUrl("NewTransferPage.qml"),{state:"download"})

        }
        ToolIcon {
            iconId: "toolbar-mediacontrol-play"
            onClicked: {if ((listTransfers.currentIndex>-1)&&(listTransfers.count>listTransfers.currentIndex)){
                    queue.resumeTransfer(listTransfers.currentIndex)

                }
                else{
                    qmgr.unpauseAllTransfers()
                }
            }
        }
        ToolIcon {
            iconId: "toolbar-mediacontrol-pause"
            onClicked: { if ((listTransfers.currentIndex>-1)&&(listTransfers.count>=listTransfers.currentIndex)){
                    queue.pauseTransfer(listTransfers.currentIndex)

                }
                else{
                    qmgr.pauseAllTransfers()
                }
            }
        }
        ToolIcon {
            id: iconBack
            iconId: "toolbar-back"
            visible: false
        }
        ToolIcon {
            id: iconMenu
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    states: [
        State {
            name: "active"
            PropertyChanges {target: refresh; running: true }
            PropertyChanges {target: iconMenu; visible: true}
            PropertyChanges {target: iconAdd; visible: true}
            PropertyChanges {target: iconDelete; visible: false}
            PropertyChanges {target: iconBack; visible: false}
        },
        State {
            name: "details"
            PropertyChanges {target: iconMenu; visible: false}
            PropertyChanges {target: iconAdd; visible: false}
            PropertyChanges {target: iconDelete; visible: true}
            PropertyChanges {target: iconBack; visible: true}

        }

    ]




}
