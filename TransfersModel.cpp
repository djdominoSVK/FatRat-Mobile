/*
FatRat download manager
http://fatrat.dolezel.info

Copyright (C) 2006-2010 Lubos Dolezel <lubos a dolezel.info>

This program is freee software; you can redistribute it and/or
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

#include <QApplication>
#include <QPainter>
#include <QMimeData>
#include <QUrl>
#include <QtDebug>

#include "TransfersModel.h"
#include "RowData.h"

extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;

using namespace std;

TransfersModel::TransfersModel()
        : QAbstractListModel(), m_queue(-1), m_nLastRowCount(0)
{
    m_states[0] = "/states/waiting.png";
    m_states[1] = "/states/active.png";
    m_states[2] = "/states/forced_active.png";
    m_states[3] = "/states/paused.png";
    m_states[4] = "/states/failed.png";
    m_states[5] = "/states/completed.png";

    m_states[6] = "/states/waiting_upload.png";
    m_states[7] = "/states/distribute.png";
    m_states[8] = "/states/forced_distribute.png";
    m_states[9] = "/states/paused_upload.png";
    m_states[10] = "/states/failed.png";
    m_states[11] = "/states/completed_upload.png";

    QHash<int, QByteArray> roles;
    roles[0] = "state";
    roles[1] = "name";
    roles[2] = "speedDown";
    roles[3] = "speedUp";
    roles[4] = "timeLeft";
    roles[5] = "message";
    roles[6] = "progress";
    roles[7] = "size";
    roles[8] = "mode";
    roles[9] = "primaryMode";
    roles[10] = "fProgress";
    roles[11] = "speed";
    roles[12] = "destination";
    roles[13] = "source";

    setRoleNames(roles);
}




QModelIndex TransfersModel::index(int row, int column, const QModelIndex &parent) const
{
	if(!parent.isValid())
		return createIndex(row,column,(void*)this);
	else
		return QModelIndex();
}

int TransfersModel::rowCount(const QModelIndex &parent) const
{
	int count = 0;
	if(!parent.isValid())
	{
		g_queuesLock.lockForRead();
		
		if(m_queue < g_queues.size() && m_queue >= 0)
			count = g_queues[m_queue]->size();
		
		g_queuesLock.unlock();
	}
	
	return count;
}


void TransfersModel::refresh()
{
        int count = 0;
        Queue* q = 0;
        g_queuesLock.lockForRead();
	
        if(m_queue < g_queues.size() && m_queue >= 0)
        {
                q = g_queues[m_queue];
                count = q->size();
        }
	
        m_lastData.resize(count);
	
        QList<bool> changes;	
        if(q != 0)
        {
                q->lock();
                for(int i=0,j=0;i<count;i++,j++)
                {
                        Transfer* t = q->at(i);
                        RowData* newData;			
                        if(t != 0)
                                newData = new RowData(t);			
                        changes << (newData != (&m_lastData[j])) ;
                        m_lastData[j] = *newData;
                }
                q->unlock();
        }
        g_queuesLock.unlock();
	
        if(count > m_nLastRowCount)
        {
                qDebug() << "Adding" << count - m_nLastRowCount << "rows";
                beginInsertRows(QModelIndex(), m_nLastRowCount, count-1);
                endInsertRows();
        }
        else if(count < m_nLastRowCount)
        {
                qDebug() << "Removing" << m_nLastRowCount - count << "rows";
                beginRemoveRows(QModelIndex(), count, m_nLastRowCount-1);
                endRemoveRows();
        }
        m_nLastRowCount = count;	
        for(int i=0;i<count;)
        {
                if(!changes[i])
                        i++;
                else
                {
                        int from = i, to;
                        while(i < count && changes[i])
                                to = i++;
			
                        dataChanged(createIndex(from,0), createIndex(to,5)); // refresh the view
                }
        }
}

QModelIndex TransfersModel::parent(const QModelIndex&) const
{
	return QModelIndex();
}

bool TransfersModel::hasChildren (const QModelIndex & parent) const
{
	return !parent.isValid();
}


QVariant TransfersModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > m_lastData.count())
        return QVariant();

    switch(role)
    {
    case 0:
    {
        Transfer::State state = m_lastData[index.row()].state();
        if(m_lastData[index.row()].mode() == Transfer::Upload)
        {
            if(state == Transfer::Completed && m_lastData[index.row()].primaryMode() == Transfer::Download)
                return m_states[5];
            else
                return m_states[state+6];
        }
        else
            return m_states[state];
    }
    case 1:
        return m_lastData[index.row()].name();
    case 2:
        return m_lastData[index.row()].speedDown();
    case 3:
        return m_lastData[index.row()].speedUp();
    case 4:
        return m_lastData[index.row()].timeLeft();
    case 5:
        return m_lastData[index.row()].message();
    case 6:
        return m_lastData[index.row()].progress();
    case 7:
        return m_lastData[index.row()].size();
    case 8:
        return m_lastData[index.row()].mode();
    case 9:
        return m_lastData[index.row()].primaryMode();
    case 10:
        return m_lastData[index.row()].fProgress();
    case 11:
        return m_lastData[index.row()].speed();
    case 12:
        return m_lastData[index.row()].destination();
    case 13:
        return m_lastData[index.row()].source();
    }
    return QVariant();
}



void TransfersModel::setQueue(int q)
{
    m_queue = q;
    refresh();
}

