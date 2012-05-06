# Add more folders to ship with the application, here
folder_01.source = qml/FatRatMobile
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE1DA1080

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components
QT += xml \
      network \
      core gui declarative

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    Auth.cpp \
    LimitedSocket.cpp \
    Logger.cpp \
    Settings.cpp \
    RowData.cpp \
    QueueMgr.cpp \
    Queue.cpp \
    Proxy.cpp \
    Transfer.cpp \
    TransfersModel.cpp \
    engines/HttpClient.cpp \
    engines/GeneralDownload.cpp \
    engines/FtpUpload.cpp \
    engines/FtpClient.cpp \
    plugins/Plugins.cpp \
    SettingsMethods.cpp \
    TransferMethods.cpp \
    TransfersMethods.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    qml/FatRatMobile/Splash.qml

HEADERS += \
    Auth.h \
    LimitedSocket.h \
    RuntimeException.h \
    RowData.h \
    QueueMgr.h \
    Queue.h \
    Proxy.h \
    Logger.h \
    Settings.h \
    Transfer.h \
    TransfersModel.h \
    engines/HttpClient.h \
    engines/GeneralDownload.h \
    engines/FtpUpload.h \
    engines/FtpClient.h \
    plugins/Plugins.h \
    SettingsMethods.h \
    TransfersMethods.h
