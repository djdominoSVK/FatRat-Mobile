import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id : listViewPage
    tools: commonTools
    //state: "active"

    ListView {
        id: listTransfers

        Component {
            id: itemComponent
            TransferItem {
                width: listTransfers.width
                height: 80
                intern_filename: name
                intern_progress: fProgress
                intern_actual_speed: model.speed
                intern_running: model.state

            }
        }
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.fill: parent
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
            interval: 1000; running: true; repeat: true
            onTriggered:  itemList.refresh()
        }
    }

    Rectangle{
        width:parent.width
        height:50
        id: test
        color: "silver"
        //border.color: "black"

        Label {
            id: name
            color: "white"
            anchors.left: parent.left
            anchors.leftMargin: 5
            font.pixelSize: 32
            text: qsTr("FatRat Mobile")
        }
    }
    Label {
        id: speed
        color: "white"
        anchors.right: parent.right
        anchors.rightMargin: 5
        font.pixelSize: 32
        text: qsTr("0 kb/s")
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
            PropertyChanges {target: listTransfers; visible: false}
        }

    ]

}
