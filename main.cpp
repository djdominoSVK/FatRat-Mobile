#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <signal.h>
#include <QDeclarativeContext>
#include <QKeyEvent>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include "Queue.h"
#include "QueueMgr.h"
#include "Transfer.h"
#include "RuntimeException.h"
#include "TransfersModel.h"
#include "SettingsMethods.h"
#include "TransfersMethods.h"
#include "Settings.h"


extern QVector<EngineEntry> g_enginesDownload;
extern QVector<EngineEntry> g_enginesUpload;

static QueueMgr* g_qmgr = 0;
QString m_strSettingsPath;

Q_DECL_EXPORT int main(int argc, char *argv[])
{

    int rval;
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;


    initSettingsDefaults(m_strSettingsPath);

    initTransferClasses();

    Transfer::runEngines(true);

    qRegisterMetaType<QString*>("QString*");
    qRegisterMetaType<QByteArray*>("QByteArray*");
    qRegisterMetaType<Transfer*>("Transfer*");
    qRegisterMetaType<Transfer::TransferList>("Transfer::TransferList");

    Queue::loadQueues();

    g_qmgr = new QueueMgr;

    TransfersModel tm;
    tm.setQueue(0);
    tm.refresh();

    TransfersMethods transfersMethods;
    SettingsMethods settingsMethods;
    settingsMethods.initSettings();

    QDeclarativeContext *ctx = viewer.rootContext();
    ctx->setContextProperty("itemList", &tm);
    ctx->setContextProperty("newTransfer",&transfersMethods);
    ctx->setContextProperty("queue",g_queues[0]);
    ctx->setContextProperty("qmgr",g_qmgr);
    ctx->setContextProperty("settingsMethods",&settingsMethods);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/FatRatMobile/main.qml"));

    viewer.showExpanded();
    rval = app->exec();

    g_qmgr->exit();
    Queue::stopQueues();
    Queue::saveQueues();
    Queue::unloadQueues();
    delete g_qmgr;
    Transfer::runEngines(false);
    return rval;
}



