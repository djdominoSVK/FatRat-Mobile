import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    id: splashScreen

    signal splashScreenCompleted()

    Image {
        id: splashImage
        source: "images/splash.png"
        anchors.fill: parent
    }

    SequentialAnimation {
        id:splashanimation
        PauseAnimation { duration: 4200 }
        PropertyAnimation {
            target: splashImage
            duration: 700
            properties: "opacity"
            to:0
        }
        onCompleted: {
            splashScreen.splashScreenCompleted()
        }
    }
    Component.onCompleted: splashanimation.start()
    onSplashScreenCompleted: {
        appWindow.pageStack.pop()
    }
 }
