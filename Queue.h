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

#ifndef _FRQUEUE_H
#define _FRQUEUE_H
#include <QtXml/QDomNode>
#include <QQueue>
#include <QReadWriteLock>
#include <QList>
#include <QPair>
#include <QUuid>
#include <QThread>
#include <QDebug>
#include "Transfer.h"

class Queue;
extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;

class Queue : public QObject
{
Q_OBJECT
public:
	Queue();
	~Queue();
	
	static void stopQueues();
	static void loadQueues();
	static void saveQueues();
	static void saveQueuesAsync();
	static void unloadQueues();
	
	Q_INVOKABLE void setSpeedLimits(int down,int up) { m_nDownLimit=down; m_nUpLimit=up; }
	void speedLimits(int& down, int& up) const { down=m_nDownLimit; up=m_nUpLimit; }
	
	Q_INVOKABLE void setTransferLimits(int down = -1,int up = -1) { m_nDownTransferLimit=down; m_nUpTransferLimit=up; }
	void transferLimits(int& down,int& up) const { down=m_nDownTransferLimit; up=m_nUpTransferLimit; }
	
	Q_INVOKABLE void setName(QString name);
	Q_INVOKABLE QString name() const;
	Q_PROPERTY(QString name READ name WRITE setName)
	
	Q_INVOKABLE void setDefaultDirectory(QString path);
	Q_INVOKABLE QString defaultDirectory() const;
	Q_PROPERTY(QString defaultDirectory READ defaultDirectory WRITE setDefaultDirectory)
	
	Q_INVOKABLE void setMoveDirectory(QString path);
	Q_INVOKABLE QString moveDirectory() const;
	Q_PROPERTY(QString moveDirectory READ moveDirectory WRITE setMoveDirectory)
	
	Q_INVOKABLE QString uuid() const { return m_uuid.toString(); }
	Q_PROPERTY(QString uuid READ uuid)
	
	Q_INVOKABLE bool upAsDown() const { return m_bUpAsDown; }
	Q_INVOKABLE void setUpAsDown(bool v) { m_bUpAsDown=v; }
	Q_PROPERTY(bool upAsDown READ upAsDown WRITE setUpAsDown)
	
	Q_INVOKABLE int size();
	Q_PROPERTY(int size READ size)
	
	void lock() { m_lock.lockForRead(); }
	void lockW() { m_lock.lockForWrite(); }
	void unlock() { m_lock.unlock(); }
	
	Q_INVOKABLE Transfer* at(int r);
	
	Q_INVOKABLE void add(Transfer* d);
	void add(QList<Transfer*> d);
	
	Q_INVOKABLE int moveDown(int n, bool nolock = false);
	Q_INVOKABLE int moveUp(int n, bool nolock = false);
	Q_INVOKABLE void moveToTop(int n, bool nolock = false);
	Q_INVOKABLE void moveToBottom(int n, bool nolock = false);
	Q_INVOKABLE void moveToPos(int from, int to, bool nolock = false);
	
	Q_INVOKABLE void remove(int n, bool nolock = false);
	Q_INVOKABLE void removeWithData(int n, bool nolock = false);
	Transfer* take(int n, bool nolock = false);
	
	void autoLimits(int& down, int& up) const { down=m_nDownAuto; up=m_nUpAuto; }
	void setAutoLimits(int down, int up);
	
	bool contains(Transfer* t) const;
    Q_INVOKABLE void stopAll();
    Q_INVOKABLE void resumeAll();

    Q_INVOKABLE QString getDownSpeed(){
        int downq = 0;

        lock();

        for(int i=0;i<size();i++)
        {
            int down,up;
            at(i)->speeds(down,up);

            downq += down;

        }
        unlock();
        return QString("%1 kB/s").arg(double(downq)/1024.f, 0, 'f', 1);
    }
    Q_INVOKABLE QString getUpSpeed(){
        int upq = 0;

        lock();

        for(int i=0;i<size();i++)
        {
            int down,up;
            at(i)->speeds(down,up);

            upq += up;

        }
        unlock();
        return QString("%1 kB/s").arg(double(upq)/1024.f, 0, 'f', 1);
    }
    Q_INVOKABLE void resumeTransfer(int index)
    {
            Queue* q  = getQueue(0, false);
            //QList<int> sel = getSelection();

            if(!q) return;

           // foreach(int i,sel)
            //{
                    Transfer* d = q->at(index);
                    //if(d->state() != Transfer::Active)
                            d->setState(Transfer::Active);
           // }

            doneQueue(q);
    }
    Q_INVOKABLE void pauseTransfer(int index)
    {
            Queue* q  = getQueue(0, false);
            //QList<int> sel = getSelection();

            if(!q) return;

           // foreach(int i,sel)
            //{
                    Transfer* d = q->at(index);
                    //if(d->state() != Transfer::Active)
                            d->setState(Transfer::Paused);
           // }

            doneQueue(q);
    }

	QQueue<QPair<int,int> > speedData() const { return m_qSpeedData; }
public slots:
	bool replace(Transfer* old, Transfer* _new);
	bool replace(Transfer* old, QList<Transfer*> _new);
private:
	void loadQueue(const QDomNode& node);
	void saveQueue(QDomNode& node,QDomDocument& doc);
    void recursiveRemove(QString what);

	QString m_strName, m_strDefaultDirectory, m_strMoveDirectory;
	int m_nDownLimit,m_nUpLimit,m_nDownTransferLimit,m_nUpTransferLimit;
	int m_nDownAuto, m_nUpAuto;
	bool m_bUpAsDown;
	QUuid m_uuid;
	mutable QReadWriteLock m_lock;
public:
	// statistics
	struct Stats
	{
		int active_d, waiting_d, active_u, waiting_u;
		int down, up;
	} m_stats;
protected:

	QList<Transfer*> m_transfers;
	QQueue<QPair<int,int> > m_qSpeedData;
	
	friend class QueueMgr;

	class BackgroundSaver : public QThread
	{
	public:
		virtual void run();
	};

	static bool m_bLoaded;


protected:
    static Queue* getQueue(int index, bool lock = true)
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

    void doneQueue(Queue* q, bool unlock = true)
        {
            if(q != 0)
            {
                if(unlock)
                    q->unlock();
                g_queuesLock.unlock();
            }
        }


};

#endif
