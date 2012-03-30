import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id : listViewPage
    tools: commonTools
    //state: "details"

    ListView {
        id: listTransfers
        width:parent.width
        height: parent.height

        Component {
            id: itemComponent
            TransferItem {
                width: listTransfers.width
                height: 80
                intern_filename: name
                intern_progress: fProgress
                intern_actual_speed: model.speed
                intern_running: model.state
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listTransfers.currentIndex = index
                        listViewPage.state= "details"
                    }
                }
            }

        }


        anchors.top: headerLabel.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.left: parent.left
        clip: true
        model: itemList
        delegate: itemComponent
        focus: true

        onCurrentIndexChanged: console.log("index")
        onCurrentItemChanged: console.log("item")
        onHighlightChanged: console.log("highlight");
        onFocusChanged: console.log("focus");

        Timer {
            id: refresh
            interval: 1000; running: false; repeat: true
            onTriggered:  itemList.refresh()
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
            text: "FatRat"
        }

    }

    ToolBarLayout {
        id: commonTools
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
        //    topMargin: 10
        }
        visible: true
        ToolIcon {
            iconId: "toolbar-add"
            onClicked: pageStack.push(Qt.resolvedUrl("NewTransferPage.qml"),{state:"download"})

        }
        ToolIcon {
            iconId: "toolbar-mediacontrol-play"
            onClicked: {queue.resumeAll()
                        listViewPage.state = "active"
            }
        }
        ToolIcon {
            iconId: "toolbar-mediacontrol-pause"
            onClicked: {queue.stopAll()
                        //itemList.refresh()
                        listViewPage.state = "paused"
                       }
        }
        ToolIcon {
            iconId: "toolbar-delete"
            visible: false
            onClicked: pageStack.push(Qt.resolvedUrl("yesno.qml"))
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    states: [
        State {
            name: "active"
            PropertyChanges {target: refresh; running: true }
        },
        State {
            name: "paused"
            PropertyChanges {target: refresh; running: false }
        },
        State {
            name: "details"
         //   PropertyChanges {target: listTransfers; visible: false}
            //PropertyChanges {target: commonTools.children.objectName.; visible: false}
        }

    ]




}
