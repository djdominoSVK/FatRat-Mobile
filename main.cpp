#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "main.h"
#include <signal.h>


#include <QDeclarativeContext>
#include <QKeyEvent>
#include "Queue.h"
#include "QueueMgr.h"
#include "Transfer.h"
#include "RuntimeException.h"
#include "TransfersModel.h"
#include "TransferFactory.h"
#include "NewTransferDlg.h"
#include "Settings.h"


extern QVector<EngineEntry> g_enginesDownload;
extern QVector<EngineEntry> g_enginesUpload;

static QueueMgr* g_qmgr = 0;
static void runEngines(bool init = true);
QString m_strSettingsPath;


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    int rval;
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;
    initSettingsDefaults(m_strSettingsPath);
    initTransferClasses();
    runEngines();

    qRegisterMetaType<QString*>("QString*");
    qRegisterMetaType<QByteArray*>("QByteArray*");
    qRegisterMetaType<Transfer*>("Transfer*");
    qRegisterMetaType<Transfer::TransferList>("Transfer::TransferList");

    Queue::loadQueues();
    g_qmgr = new QueueMgr;

    TransfersModel tm;
    tm.setQueue(0);
    tm.refresh();

    QDeclarativeContext *ctx = viewer.rootContext();

    NewTransferDlg ntd(&viewer);
    ntd.initSettings();

    ctx->setContextProperty("itemList", &tm);
    ctx->setContextProperty("newTransfer",&ntd);
    ctx->setContextProperty("queue",g_queues[0]);
    ctx->setContextProperty("qmgr",g_qmgr);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/FatRatMobile/main.qml"));

    viewer.showExpanded();
    rval = app->exec();



    g_qmgr->exit();
    Queue::stopQueues();
    Queue::saveQueues();
    Queue::unloadQueues();

    delete g_qmgr;
    runEngines(false);

    return rval;
}

static void runEngines(bool init)
{
        for(int i=0;i<g_enginesDownload.size();i++)
        {
                if(init)
                {
                        if(g_enginesDownload[i].lpfnInit)
                                g_enginesDownload[i].lpfnInit();
                }
                else
                {
                        if(g_enginesDownload[i].lpfnExit)
                                g_enginesDownload[i].lpfnExit();
                }
        }

        for(int i=0;i<g_enginesUpload.size();i++)
        {
                if(init)
                {
                        if(g_enginesUpload[i].lpfnInit)
                                g_enginesUpload[i].lpfnInit();
                }
                else
                {
                        if(g_enginesUpload[i].lpfnExit)
                                g_enginesUpload[i].lpfnExit();
                }
        }
}



