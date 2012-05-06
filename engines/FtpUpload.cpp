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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
USA.
*/


#include "FtpUpload.h"
#include "RuntimeException.h"
#include <QFileInfo>
#include <QMenu>
#include <QtDebug>

FtpUpload::FtpUpload()
	: m_engine(0), m_nUpLimit(0), m_nDone(0), m_mode(FtpPassive)
{
	Transfer::m_mode = Upload;
}

FtpUpload::~FtpUpload()
{
	if(m_engine != 0)
		m_engine->destroy();
}

int FtpUpload::acceptable(QString url, bool bDrop)
{
	if(bDrop)
		return (url.startsWith("file://") || url.startsWith("/")) ? 2 : 0;
	else
		return (
			url.startsWith("ftp://")
#ifdef WITH_SFTP
			|| url.startsWith("sftp://")
#endif
		       ) ? 2 : 0;
}

void FtpUpload::init(QString source, QString target)
{
	QFileInfo finfo;
	
	if(source.startsWith("file:///"))
		source = source.mid(7);
	
	finfo = QFileInfo(source);
	
	if(!finfo.exists())
		throw RuntimeException(tr("File does not exist"));
	
	m_nTotal = finfo.size();
	if(m_strName.isEmpty())
		m_strName = finfo.fileName();
	
    if(!target.startsWith("ftp://"))
		throw RuntimeException(tr("Invalid protocol for this upload class (FTP)"));
	
	m_strTarget = target;
	m_strSource = source;
	
}

void FtpUpload::setObject(QString object)
{
	m_strSource = object;
}

void FtpUpload::changeActive(bool nowActive)
{
	if(nowActive)
	{
		m_strMessage = QString();
		
		if(m_strTarget.scheme() == "ftp")
			m_engine = new FtpEngine(m_strTarget, m_proxy);
#ifdef WITH_SFTP
		else if(m_strTarget.scheme() == "sftp")
			m_engine = new SftpEngine(m_strTarget);
#endif
		else
		{
			m_strMessage = tr("Unsupported protocol");
			setState(Failed);
			return;
		}
		
		connect(m_engine, SIGNAL(finished(bool)), this, SLOT(finished(bool)));
		connect(m_engine, SIGNAL(logMessage(QString)), this, SLOT(enterLogMessage(QString)));
		m_engine->setRemoteName(m_strName);
		m_engine->request(m_strSource, true, (m_mode == FtpActive) ? FtpEngine::FtpActive : FtpEngine::FtpPassive);
	}
	else if(m_engine != 0)
	{
		safeDestroy();
	}
}

void FtpUpload::safeDestroy()
{
	LimitedSocket* engine = m_engine;
	if(engine != 0)
	{
		m_engine = 0;
		engine->destroy();
	}
}

void FtpUpload::finished(bool error)
{
	if(error)
	{
		m_strMessage = m_engine->errorString();
		setState(Failed);
	}
	else
	{
		m_nDone = m_nTotal;
		setState(Completed);
	}
	
	safeDestroy();
}

void FtpUpload::status(QString str)
{
	m_strMessage = str;
}

qulonglong FtpUpload::done() const
{
	if(m_engine != 0)
	{
		qulonglong d = m_engine->done();
		if(d > 0)
			return m_nDone = d;
	}
	return m_nDone;
}

void FtpUpload::setSpeedLimits(int,int up)
{
	m_nUpLimit = up;
	if(m_engine != 0)
		m_engine->setLimit(up);
}

void FtpUpload::speeds(int& down, int& up) const
{
	up = down = 0;
	if(m_engine != 0)
		up = m_engine->speed();
}

void FtpUpload::load(const QDomNode& map)
{
	QString source, target;
	Transfer::load(map);
	
	source = getXMLProperty(map, "source");
	target = getXMLProperty(map, "target");
	m_strName = getXMLProperty(map, "name");
	m_mode = (FtpMode) getXMLProperty(map, "ftpmode").toInt();
	m_nDone = getXMLProperty(map, "done").toLongLong();
	m_proxy = getXMLProperty(map, "proxy");
	
	try
	{
		init(source, target);
	}
	catch(const RuntimeException& e)
	{
		setState(Failed);
		m_strMessage = e.what();
	}
}

void FtpUpload::save(QDomDocument& doc, QDomNode& map) const
{
	Transfer::save(doc, map);
	
	setXMLProperty(doc, map, "source", m_strSource);
	setXMLProperty(doc, map, "target", m_strTarget.toString());
	setXMLProperty(doc, map, "name", m_strName);
	setXMLProperty(doc, map, "ftpmode", QString::number(m_mode));
	setXMLProperty(doc, map, "done", QString::number(m_nDone));
	setXMLProperty(doc, map, "proxy", m_proxy.toString());
}



