import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id : listViewPage
    tools: commonTools

    Component {
        id: transferDelegate

        Item {
            id: transfer

            property real detailsOpacity : 0

            width: listTransfers.width
            height: 80

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listTransfers.currentIndex = index
                    transfer.state = 'Details'
                    listViewPage.state = "details"
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
                opacity: transfer.detailsOpacity


                Label {
                    id: progressLabel
                    anchors.top: parent.top
                    anchors.topMargin: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
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
                    id: timeLeftLabel
                    anchors.top: progressLabel.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    font.pixelSize: 24
                    text: "Time left: "
                }
                Text {
                    id: transferTimeLeft
                    anchors.top: progressLabel.bottom
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
            }

            // A button to close the detailed view, i.e. set the state back to default ('').
//            Button {
//                y: 10
//                anchors { right: parent.right; rightMargin: 10; bottom: parent.bottom; bottomMargin: 50}
//                opacity: transfer.detailsOpacity
//                text: "Close"

//                onClicked: transfer.state = '';
//            }

            states: State {
                name: "Details"

               // PropertyChanges { target: background; color: "white" }
            //    PropertyChanges { target: iconImage; width: 130; height: 130 } // Make picture bigger
                PropertyChanges {target: iconBack; onClicked:{
                        transfer.state = ''
                        iconMenu.visible = true
                        iconAdd.visible = true
                        iconDelete.visible = false
                        iconBack.visible= false
                        listViewPage.state = "active"
                        listTransfers.currentIndex = -1}}
                PropertyChanges { target: transfer; detailsOpacity: 1; x: 0 } // Make details visible
                PropertyChanges { target: transfer; height:  listTransfers.height } // Fill the entire list area with the detailed view

                // Move the list so that this item is at the top.
                PropertyChanges { target: transfer.ListView.view; explicit: true; contentY: transfer.y }

                // Disallow flicking while we're in detailed view
                PropertyChanges { target: transfer.ListView.view; interactive: false }
            }

            transitions: Transition {
                // Make the state changes smooth
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
        clip: true
        model: itemList
        delegate: transferDelegate



        onCurrentIndexChanged: {console.log("index "+ listTransfers.currentIndex)}
        onCurrentItemChanged: console.log("item")
        Timer {
            id: refresh
            interval: 2000; running: true; repeat: true
            onTriggered:  {itemList.refresh()
                downloadText.text = "Download: " + queue.getDownSpeed()
                uploadText.text = "Upload: " + queue.getUpSpeed()
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
        id: name
        height: 50
        width: parent.width
        Text {
          id: text
          font.pixelSize: 22
          anchors.centerIn: parent
          color: "white"
          text: "Really remove this transfer?"
        }
      }

      buttons:  ButtonColumn {
          spacing: 15
          style: ButtonStyle { }
          anchors.horizontalCenter: parent.horizontalCenter
          Button {text: "Delete with data"; onClicked:
              {if ((listTransfers.currentIndex>-1)&&(listTransfers.count>listTransfers.currentIndex)){
                                         queue.removeWithData(listTransfers.currentIndex,false)

                                     }
                                     else{
                                            deleteTransferDialog.close()
                                     }
                  listTransfers.currentItem.state = ''
                  iconMenu.visible = true
                  iconAdd.visible = true
                  iconDelete.visible = false
                  iconBack.visible= false
                  listViewPage.state = "active"
                  listTransfers.currentIndex = -1
                  deleteTransferDialog.accept()
              }
          }
          Button {id: but1 ;text: "Delete without data"; onClicked:
              {if ((listTransfers.currentIndex>-1)&&(listTransfers.count>listTransfers.currentIndex)){
                                         queue.remove(listTransfers.currentIndex,false)

                                     }
                                     else{
                                            deleteTransferDialog.close()
                                     }
                  listTransfers.currentItem.state = ''
                  iconMenu.visible = true
                  iconAdd.visible = true
                  iconDelete.visible = false
                  iconBack.visible= false
                  listViewPage.state = "active"
                  listTransfers.currentIndex = -1
                  deleteTransferDialog.accept()
              }
          }
          Button {text: "Cancel"; onClicked: deleteTransferDialog.close()}
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
                            queue.resumeAll()
                            listViewPage.state = "active"
                }
                        //refreshHeader.running=true
            }
        }
        ToolIcon {
            iconId: "toolbar-mediacontrol-pause"
            onClicked: { if ((listTransfers.currentIndex>-1)&&(listTransfers.count>=listTransfers.currentIndex)){
                            queue.pauseTransfer(listTransfers.currentIndex)

                            }
                       else{
                            queue.stopAll()
                            itemList.refresh()

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
           // PropertyChanges {target: refreshHeader; running: true }
        },
        State {
            name: "paused"
          //  PropertyChanges {target: refresh; running: false }
          //  PropertyChanges {target: refreshHeader; running: false }
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
