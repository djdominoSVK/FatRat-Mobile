#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "main.h"
#include <signal.h>


#include <QDeclarativeContext>
#include <QKeyEvent>
#include "Queue.h"
#include "Transfer.h"
#include "RuntimeException.h"
#include "TransfersModel.h"
#include "TransferFactory.h"
#include "NewTransferDlg.h"


extern QVector<EngineEntry> g_enginesDownload;
extern QVector<EngineEntry> g_enginesUpload;


static void runEngines(bool init = true);


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    int rval;
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QmlApplicationViewer viewer;

    initTransferClasses();
    runEngines();

    qRegisterMetaType<QString*>("QString*");
    qRegisterMetaType<QByteArray*>("QByteArray*");
    qRegisterMetaType<Transfer*>("Transfer*");
    qRegisterMetaType<Transfer::TransferList>("Transfer::TransferList");

    Queue::loadQueues();

    TransfersModel tm;
    tm.setQueue(0);
    tm.refresh();

    QDeclarativeContext *ctx = viewer.rootContext();

    NewTransferDlg ntd(&viewer);

    ctx->setContextProperty("itemList", &tm);
    ctx->setContextProperty("newTransfer",&ntd);
    ctx->setContextProperty("queue",g_queues[0]);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/FatRatMobile/main.qml"));

    viewer.showExpanded();
    rval = app->exec();

    Queue::stopQueues();
    Queue::saveQueues();
    Queue::unloadQueues();

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


