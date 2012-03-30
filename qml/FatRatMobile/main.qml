import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: listViewPage

    MainPage {
        id : listViewPage
        tools: commonTools
    }

    ToolBarLayout {
        id: commonTools
        anchors {
        //    horizontalCenter: parent.horizontalCenter
            top: parent.top
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

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Accounts")
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingPage.qml"))
            }
            MenuItem {
                text: qsTr("About")
            }
            MenuItem {
                text: qsTr("Exit")
            }
        }
    }
}
