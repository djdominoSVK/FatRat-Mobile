import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    id: container

    property string fontName: "Helvetica"
    property int fontSize: 12
    property color fontColor: "black"
    property double margins: 8
    property int settingHeight: visual.theme.settingHeight
    // Selected theme button
    property variant __selectedButton: darkButton

    signal themeChanged(string theme)

    Column {
        anchors.fill: parent
        anchors.margins: container.margins
        spacing: container.margins

        Item {
            height: container.settingHeight
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                id: switchLabel

                anchors.left: parent.left
                anchors.right: themeSwitch.left
                height: container.settingHeight
                color: container.fontColor
                verticalAlignment: Text.AlignVCenter
                font {
                    family: container.fontName
                    pointSize: container.fontSize
                }

                text: themeSwitch.checkedButton === lightButton ?
                          qsTr("Theme: Light") : qsTr("Theme: Dark")
            }

            ButtonRow {
                id: themeSwitch

                anchors.right: parent.right
                width: 266

                Button {
                    id: darkButton
                    width: visual.theme.segmentedButtonWidth
                    text: qsTr("Dark")
                    onClicked: {
                        if (container.__selectedButton != darkButton) {
                            switchLabel.text = qsTr("Theme: Dark");
                            container.__selectedButton = darkButton;
                            themeChanged("DarkTheme");
                        }
                    }
                }
                Button {
                    id: lightButton
                    width: visual.theme.segmentedButtonWidth
                    text: qsTr("Light")
                    onClicked: {
                        if (container.__selectedButton != lightButton) {
                            switchLabel.text = qsTr("Theme: Light");
                            container.__selectedButton = lightButton;
                            themeChanged("Visual");
                        }
                    }
                }
            }
        }
    }

    onStatusChanged: {
        if (container.status === PageStatus.Active) {
            themeSwitch.checkedButton = container.__selectedButton;
        }
    }

    Component.onCompleted: {
        // Restore application settings values on startup.
        var inverted = Store.restoreSettings();
        if (inverted == "true") {
            // Set the light theme.
            container.__selectedButton = lightButton;
            themeChanged("Visual");
        }
    }

    Component.onDestruction: {
        // Save application settings values on exit.
        Store.storeSettings(appState.isInverted);
    }
}
