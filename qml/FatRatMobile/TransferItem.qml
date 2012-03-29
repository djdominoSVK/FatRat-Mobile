import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    id: itemComponent

    property string intern_filename
    property string intern_running
    property real intern_progress
    property string intern_actual_speed

    Image {
        id: icon
        width: 42
        height: 42
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 15
        source: ("images" + intern_running)
    }

    Text {
        id: filenameTxt
        font.family: "Tahoma"
        color: "black"
        font.pixelSize: 24
        elide: Text.ElideRight
        text: intern_filename
        anchors.left: icon.right
        anchors.leftMargin: 5
        anchors.right: speedTxt.left
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    ProgressBar {
        id: progressBar
        value: intern_progress
        maximumValue: 100
        minimumValue: 0
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: icon.bottom
        anchors.topMargin: 4
    }

    Text {
        id: speedTxt
        font.family: "Tahoma"
        color: "black"
        font.pixelSize: 24
        text: if(intern_running) { return intern_actual_speed } else { return "n/a" }
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 20
    }

}
