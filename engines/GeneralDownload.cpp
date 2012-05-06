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

#include "GeneralDownload.h"
#include "RuntimeException.h"
#include "HttpClient.h"
#include "FtpClient.h"


#include <QtDebug>
#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QSettings>
#include <QMessageBox>
#include <QMenu>


#include <QtNetwork/QNetworkReply>
#include <qeventloop.h>
#include <QStringList>

#include <plugins/Plugins.h>

using namespace std;

extern QSettings* g_settings;

GeneralDownload::GeneralDownload(bool local) : Transfer(local), m_nTotal(0), m_nStart(0),
m_bAutoName(false), m_engine(0), m_nUrl(0)
{
}

GeneralDownload::~GeneralDownload()
{
	if(m_engine)
		m_engine->destroy();
}

int GeneralDownload::acceptable(QString uri, bool)
{
	QUrl url = uri;
	QString scheme = url.scheme();
	
	if(scheme != "http" && scheme != "ftp"
#ifdef WITH_SFTP
		&& scheme != "sftp"
#endif
		&& scheme != "https")
		return 0;
	else
        return 2;
}

QString GeneralDownload::name() const
{
	return m_strFile;
}

void GeneralDownload::init(QString uri,QString dest)
{
	UrlObject obj;
	
	int hash = uri.lastIndexOf("#__filename=");
	if(hash != -1)
	{
		QString name = QUrl::fromPercentEncoding(uri.mid(hash+12).toUtf8());
		m_strFile = name;
		uri.resize(hash);
	}
	hash = uri.indexOf('#');
	if(hash != -1)
		uri.resize(hash);
	
	obj.url = uri;
	
    obj.proxy = g_settings->value("httpftp/defaultproxy").toString();
	obj.ftpMode = FtpPassive;
	
	m_dir = dest;
	
	QString scheme = obj.url.scheme();
	if(scheme != "http" && scheme != "ftp" && scheme != "sftp" && scheme != "https")
		throw RuntimeException(tr("Unsupported protocol"));
	
	m_urls.clear();
	m_urls << obj;
	
	if(m_strFile.isEmpty())
		generateName();
}

void GeneralDownload::generateName()
{
	QString name;
	if(!m_urls.isEmpty())
		name = QFileInfo(m_urls[0].url.path()).fileName();
	m_strFile = (!name.isEmpty()) ? name : "default.html";
	m_bAutoName = true;
}

void GeneralDownload::setObject(QString target)
{
	QDir dirnew = target;
	if(dirnew != m_dir)
	{
		m_dir = dirnew;
		if(isActive())
			setState(Waiting); // restart download
	}
}

void GeneralDownload::init2(QString uri,QString dest)
{
	QDir dir(dest);
	m_strFile = dir.dirName();
	dir.cdUp();
	init(uri, dir.path());
}

void GeneralDownload::speeds(int& down, int& up) const
{
	up = down = 0;
	if(isActive())
	{
		if(m_engine)
			down = m_engine->speed();
	}
}

void GeneralDownload::load(const QDomNode& map)
{
	m_dir = getXMLProperty(map, "dir");
	m_nTotal = getXMLProperty(map, "knowntotal").toULongLong();
	m_strFile = getXMLProperty(map, "filename");
	m_bAutoName = getXMLProperty(map, "autoname").toInt() != 0;
	
	QDomElement url = map.firstChildElement("url");
	while(!url.isNull())
	{
		UrlObject obj;
		
		obj.url = getXMLProperty(url, "address");
		obj.strReferrer = getXMLProperty(url, "referrer");
		obj.proxy = getXMLProperty(url, "proxy");
		obj.ftpMode = (FtpMode) getXMLProperty(url, "ftpmode").toInt();
		obj.strBindAddress = getXMLProperty(url, "bindip");
		
		url = url.nextSiblingElement("url");
		
		m_urls << obj;
	}
	
	if(m_strFile.isEmpty())
		generateName();
	
	Transfer::load(map);
}

void GeneralDownload::save(QDomDocument& doc, QDomNode& map) const
{
	Transfer::save(doc, map);
	
	setXMLProperty(doc, map, "dir", m_dir.path());
	setXMLProperty(doc, map, "knowntotal", QString::number(m_nTotal));
	setXMLProperty(doc, map, "filename", m_strFile);
	setXMLProperty(doc, map, "autoname", QString::number(m_bAutoName));
	
	for(int i=0;i<m_urls.size();i++)
	{
		QDomElement sub = doc.createElement("url");
		//sub.setAttribute("id", QString::number(i));
		
		setXMLProperty(doc, sub, "address", m_urls[i].url.toString());
		setXMLProperty(doc, sub, "referrer", m_urls[i].strReferrer);
		setXMLProperty(doc, sub, "proxy", m_urls[i].proxy.toString());
		setXMLProperty(doc, sub, "ftpmode", QString::number( (int) m_urls[i].ftpMode ));
		setXMLProperty(doc, sub, "bindip", m_urls[i].strBindAddress);
		
		map.appendChild(sub);
	}
}

qulonglong GeneralDownload::done() const
{
	return QFileInfo(m_dir.filePath(name())).size();
}

void GeneralDownload::changeActive(bool nowActive)
{
	if(nowActive)
	{
		if(m_urls.isEmpty())
		{
			m_strMessage = tr("No URLs have been given");
			enterLogMessage(tr("No URLs have been given"));
		}
		else
		{
			if(m_nUrl >= m_urls.size())
				m_nUrl = 0;
			
			enterLogMessage(tr("Downloading URL %1").arg(m_urls[m_nUrl].url.toString()));
			
			m_strMessage.clear();
			m_urlLast = QUrl();
			
			QString scheme = m_urls[m_nUrl].url.scheme();
			
            if(scheme == "http" || scheme == "https" || (Proxy::getProxy(m_urls[m_nUrl].proxy).nType == Proxy::ProxyHttp && scheme == "ftp"))
                startHttp(m_urls[m_nUrl].url,m_urls[m_nUrl].strReferrer,false);
			else if(scheme == "ftp")
				startFtp(m_urls[m_nUrl].url);
#ifdef WITH_SFTP
			else if(scheme == "sftp")
				startSftp(m_urls[m_nUrl].url);
#endif
			else
			{
				m_strMessage = tr("Unsupported protocol");
				setState(Failed);
				return;
			}
			
			int down, up;
			internalSpeedLimits(down, up);
			m_engine->setLimit(down);
		}
	}
	else
	{
		if(m_engine)
		{
			m_engine->destroy();
			m_engine = 0;
		}
	}
}

void GeneralDownload::connectSignals()
{
	connect(m_engine, SIGNAL(receivedSize(qint64)), this, SLOT(responseSizeReceived(qint64)), Qt::DirectConnection);
	connect(m_engine, SIGNAL(finished(bool)), this, SLOT(requestFinished(bool)), Qt::QueuedConnection);
	connect(m_engine, SIGNAL(statusMessage(QString)), this, SLOT(changeMessage(QString)));
	connect(m_engine, SIGNAL(logMessage(QString)), this, SLOT(enterLogMessage(QString)));
}

void GeneralDownload::startHttp(QUrl url, QUrl referrer,bool redirected)
{
	qDebug() << "GeneralDownload::startHttp" << url;
	
    if (!redirected){
        Plugins plugins;
        qDebug() << url.host();
        QString realUrl = plugins.checkUrl(url);
        if (realUrl != NULL){
            if (realUrl != "unknown_server"){
                url.setUrl(realUrl);
            }
            if (realUrl == "failed"){
                url.setUrl("");
            }
        }
    }
    m_urlLast = url;
    m_engine = new HttpEngine(url, referrer, m_urls[m_nUrl].proxy);
	
	connect(m_engine, SIGNAL(redirected(QString)), this, SLOT(redirected(QString)));
	connect(m_engine, SIGNAL(renamed(QString)), this, SLOT(renamed(QString)));
	connectSignals();
	
	m_engine->bind(QHostAddress(m_urls[m_nUrl].strBindAddress));
	m_engine->request(filePath(), false, 0);
}



void GeneralDownload::startFtp(QUrl url)
{
	qDebug() << "GeneralDownload::startFtp" << url;
	
	m_urlLast = url;
	m_engine = new FtpEngine(url, m_urls[m_nUrl].proxy);
	
	connectSignals();
	
	m_engine->request(filePath(), false, (m_urls[m_nUrl].ftpMode == FtpActive ? FtpEngine::FtpActive : FtpEngine::FtpPassive));
}


void GeneralDownload::responseSizeReceived(qint64 totalsize)
{
	m_nTotal = totalsize;
}

void GeneralDownload::requestFinished(bool error)
{
	void* obj = sender();
	
	if(isActive() && m_engine == obj)
	{
		m_strMessage = ((LimitedSocket*) obj)->errorString();
		if(error)
		{
			enterLogMessage(tr("Transfer has failed: %1").arg(m_strMessage));
			setState(Failed);
		}
		else if(isActive())
		{
			if(m_engine)
			{
				m_engine->destroy();
				m_engine = 0;
			}
			
			enterLogMessage(tr("Transfer has been completed"));
			setState(Completed);
		}
	}
}

void GeneralDownload::redirected(QString newurl)
{
	if(!newurl.isEmpty())
	{
		QUrl location = newurl;
		QString scheme = location.scheme();
		
		location.setUserInfo(m_urlLast.userInfo());
		
		{
			QUrl tmploc = location;
			tmploc.setUserInfo(QString());
			qDebug() << "Redirected to: " << tmploc << endl;
			enterLogMessage(tr("We're being redirected to: %1").arg(tmploc.toString()));
		}
		
		if(scheme == "http" || scheme == "ftp" || scheme == "https")
		{
			if(m_bAutoName)
			{
				QString newname = QFileInfo(newurl).fileName();
				if(!newname.isEmpty())
					setTargetName(newname);
			}
			
			if(scheme == "http" || scheme == "https")
                startHttp(location, m_urlLast,true);
			else
				startFtp(location);
			
			m_strMessage = tr("Redirected");
		
			int down, up;
			internalSpeedLimits(down, up);
			m_engine->setLimit(down);
		}
		else
		{
			m_strMessage = tr("Invalid redirect");
			enterLogMessage(tr("We've been redirected to an unsupported URL: %1").arg(location.toString()));
			setState(Failed);
		}
	}
	else
	{
		m_strMessage = tr("Invalid redirect");
		enterLogMessage(tr("We've been redirected, but no new URL has been given"));
		setState(Failed);
	}
}

void GeneralDownload::setSpeedLimits(int down,int)
{
	if(down < 1000)
		down = 0;
	if(m_engine)
		m_engine->setLimit(down);
}

void GeneralDownload::renamed(QString dispName)
{
	if(m_bAutoName)
	{
		dispName.replace('/', '_');
		qDebug() << "Automatically renaming to" << dispName;
		setTargetName(dispName);
	}
}

void GeneralDownload::setTargetName(QString newFileName)
{
	if(m_strFile != newFileName)
	{
		m_dir.rename(m_strFile, newFileName);
		m_strFile = newFileName;
	}
}

void GeneralDownload::switchMirror()
{
	int prev,cur;
	prev = cur = m_nUrl;
	
	cur++;
	if(cur >= m_urls.size())
		cur = 0;
	
	if(cur == prev)
		enterLogMessage(tr("No mirror to switch to!"));
	else
	{
		enterLogMessage(tr("Switching mirror: %1 -> %2").arg(m_urls[prev].url.toString()).arg(m_urls[cur].url.toString()));
		m_nUrl = cur;
		
		if(isActive())
		{
			changeActive(false);
			changeActive(true);
		}
	}
}
