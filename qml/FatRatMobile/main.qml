import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: 0
    Component.onCompleted: {
                pageStack.push([
                        {page: Qt.resolvedUrl("MainPage.qml")},
                        {page: Qt.resolvedUrl("Splash.qml")}]);
            }

    MainPage {
        id : listViewPage
    }


    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
            MenuItem {
                text: qsTr("Proxy")
                onClicked: pageStack.push(Qt.resolvedUrl("ProxyPage.qml"))
            }

        }
    }
}
