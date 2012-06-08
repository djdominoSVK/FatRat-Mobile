#include "TransfersMethods.h"
#include "Queue.h"
#include "Transfer.h"

#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QReadWriteLock>
#include <QMenu>
#include <QClipboard>
#include <QtDebug>
#include <QUrl>
#include "RuntimeException.h"

extern QSettings* g_settings;
extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;
extern QVector<EngineEntry> g_enginesDownload;
extern QVector<EngineEntry> g_enginesUpload;


/**
 * @brief simple constructor
 *
 * @param parent
 */
TransfersMethods::TransfersMethods(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief creating new transfer
 *
 * @param m_strURIs transfers urls
 * @param downloadTrueUploadFalse boolean value for download or upload
 * @param m_nClass download type default -1
 * @param m_strDestination destination fo transfer
 * @param m_nDownLimit down limit of transfer
 * @param m_nUpLimit up limit of transfer
 * @param m_bPaused boolean true if transfer is paused
 */
void TransfersMethods::createTransfer(QString m_strURIs,bool downloadTrueUploadFalse,int m_nClass,QString m_strDestination,
                                     int m_nDownLimit,int m_nUpLimit,bool m_bPaused)
{
     Queue* queue = 0;
     QList<Transfer*> listTransfers;
     try
     {
         QStringList uris;
             uris = m_strURIs.split('\n', QString::SkipEmptyParts);
         if(uris.isEmpty())
             throw RuntimeException();
         for(int i=0;i<uris.size();i++)
         {
             QString trm = uris[i].trimmed();

             if(trm.isEmpty())
             {
                 uris.removeAt(i);
                 i--;
             }
             else
                 uris[i] = trm;
         }
         int detectedClass = m_nClass;
         for(int i=0;i<uris.size();i++)
         {
             Transfer* d;

             int classID;
             if(m_nClass == -1)
             {
                 Transfer::BestEngine eng;

                 if(downloadTrueUploadFalse)
                     eng = Transfer::bestEngine(uris[i], Transfer::Download);
                 else
                     eng = Transfer::bestEngine(m_strDestination, Transfer::Upload);

                 if(eng.nClass < 0)
                     throw RuntimeException(tr("Couldn't autodetect transfer type for \"%1\"").arg(uris[i]));
                 else
                     classID = eng.nClass;

                 if(detectedClass == -1)
                     detectedClass = classID;
                 else if(detectedClass >= 0 && detectedClass != classID)
                     detectedClass = -2;
             }
             else
                 classID = m_nClass;

             Transfer::Mode myMode = downloadTrueUploadFalse ? Transfer::Download : Transfer::Upload;;
             d = Transfer::createInstance(myMode, classID);

             if(d == 0)
                 throw RuntimeException(tr("Failed to create a class instance."));

             listTransfers << d;

             QString source, destination;

             source = uris[i].trimmed();
             destination = m_strDestination;

             d->setUrl( (myMode == Transfer::Download) ? source : destination);
             if(!m_auth.strUser.isEmpty())
             {
                 QString& obj = (myMode == Transfer::Download) ? source : destination;

                 QUrl url = obj;
                 if(url.userInfo().isEmpty())
                 {
                     url.setUserName(m_auth.strUser);
                     url.setPassword(m_auth.strPassword);
                 }
                 obj = url.toString();
             }


             d->init(source, destination);
             d->setUserSpeedLimits(m_nDownLimit,m_nUpLimit);
         }
         if(m_bPaused)
         {
             foreach(Transfer* d, listTransfers)
                 d->setState(Transfer::Waiting);
         }
         else
         {
             foreach(Transfer* d, listTransfers)
                 d->setState(Transfer::Active);
         }
         g_queuesLock.lockForWrite();
         queue = g_queues[0];
         if(!queue)
             throw RuntimeException(tr("Internal error."));
         queue->add(listTransfers);
         g_queuesLock.unlock();
     }
     catch(const RuntimeException& e)
     {
         qDeleteAll(listTransfers);
         listTransfers.clear();
         if(!e.what().isEmpty())
         {
             qDebug() <<"Error: " + e.what();
         }
     }

     Queue::saveQueuesAsync();

}


/**
 * @brief getting download links form file
 *
 * @param filename source file
 * @return QString  links for file
 */
QString TransfersMethods::getLinksFromFile(QString filename ){
    if(!filename.isNull())
    {
        QFile file(removeFileFromString(filename));
        if(file.open(QIODevice::ReadOnly)){

            QString output = file.readAll();
            return output;
        }
        return "";

    }
    return "";
}

/**
 * @brief removing tag "file://" from qstring
 *
 * @param filename source name to remove tag
 * @return QString returns name without tag "file://"
 */
QString TransfersMethods::removeFileFromString(QString filename ){
    QStringList list = filename.split("file://");
    if (list.size()>1){
        return list[1];
    }
    else return NULL;
}

/**
 * @brief setting up user
 *
 * @param regExp name of server useless in this implementation
 * @param user login
 * @param pass password
 */
void TransfersMethods::authData(QString regExp, QString user, QString pass) {
    m_auth.strRegExp = regExp;
    m_auth.strUser = user;
    m_auth.strPassword = pass;
}

/**
 * @brief returns login
 *
 * @return QString login
 */
QString TransfersMethods::authUsername(){
    return m_auth.strUser;
}

/**
 * @brief returns password
 *
 * @return QString password
 */
QString TransfersMethods::authPassword(){
    return m_auth.strPassword;
}


