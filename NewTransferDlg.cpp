/*
FatRat download manager
http://fatrat.dolezel.info

Copyright (C) 2006-2008 Lubos Dolezel <lubos a dolezel.info>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

In addition, as a special exemption, Luboš Doležel gives permission
to link the code of FatRat with the OpenSSL project's
"OpenSSL" library (or with modified versions of it that use the; same
license as the "OpenSSL" library), and distribute the linked
executables. You must obey the GNU General Public License in all
respects for all of the code used other than "OpenSSL".
*/

#include "NewTransferDlg.h"
#include "Queue.h"
#include "TransfersModel.h"

#include <QApplication>
#include <QFileDialog>
#include <QSettings>
#include <QReadWriteLock>
#include <QMenu>
#include <QClipboard>
#include <QtDebug>
#include "RuntimeException.h"

extern QSettings* g_settings;
extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;
extern QVector<EngineEntry> g_enginesDownload;
extern QVector<EngineEntry> g_enginesUpload;



void NewTransferDlg::createTransfer(QString m_strURIs,bool downloadTrueUploadFalse,int m_nClass,QString m_strDestination,
                                     int m_nDownLimit,int m_nUpLimit,bool m_bPaused)
{
     Queue* queue = 0;
     QStringList test;
        for(int i=0;i<g_enginesDownload.size();i++)
            test<<(g_enginesDownload[i].longName);

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
                 // autodetection
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
            queue = getQueue(0, false);
         if(!queue)
             throw RuntimeException(tr("Internal error."));

         queue->add(listTransfers);
     }
     catch(const RuntimeException& e)
     {
         qDeleteAll(listTransfers);
         listTransfers.clear();
         if(!e.what().isEmpty())
         {
             //qDebug <<"Error: " + e.what();
         }
     }

     if(queue != 0)
         doneQueue(queue,false);

     Queue::saveQueuesAsync();

}

 void NewTransferDlg::doneQueue(Queue* q, bool unlock, bool refresh)
 {
     if(q != 0)
     {
         if(unlock)
             q->unlock();
         g_queuesLock.unlock();
     }
 }

 Queue* NewTransferDlg::getQueue(int index, bool lock)
 {
     g_queuesLock.lockForRead();

     if(index < 0 || index >= g_queues.size())
     {
         if(index != -1)
             qDebug() << "MainWindow::getQueue(): Invalid queue requested: " << index;
         g_queuesLock.unlock();
         return 0;
     }

     Queue* q = g_queues[index];
     if(lock)
         q->lock();
     return q;
 }

QString NewTransferDlg::addTextFile()
{
	QString filename = QFileDialog::getOpenFileName(this, "FatRat");
	if(!filename.isNull())
	{
		QFile file(filename);
        if(file.open(QIODevice::ReadOnly)){

            QString output = file.readAll();
            return output;
        }
        return "";

	}
    return "";
}


QString NewTransferDlg::browse(QString current)
{
    QString dir = QFileDialog::getExistingDirectory(this, "FatRat", current);
	if(!dir.isNull())
        return dir;
    return "";
}

QString NewTransferDlg::browse2()
{
	QStringList files;
	
	files = QFileDialog::getOpenFileNames(this, "FatRat");
    QString output = files.join(" \n ");
    return output;
}


