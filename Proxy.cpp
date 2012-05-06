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

#include "Proxy.h"
#include <QSettings>
#include <QtNetwork/QHttp>
#include <QtDebug>

extern QSettings* g_settings;

Proxy Proxy::loadProxy()
{
    Proxy p;
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    p.strName = g_settings->value("name").toString();
    p.strIP = g_settings->value("ip").toString();
    p.nPort = g_settings->value("port").toUInt();
    p.strUser = g_settings->value("user").toString();
    p.strPassword = g_settings->value("password").toString();
    p.nType = (Proxy::ProxyType) g_settings->value("type",0).toInt();
    p.uuid = g_settings->value("uuid").toString();
    p.enabled = g_settings->value("enabled").toBool();
    g_settings->endArray();
    return p;
}

Proxy Proxy::getProxy(QUuid uuid)
{
    if(uuid.isNull())
        return Proxy();
    g_settings->beginReadArray("httpftp/proxys");

    Proxy p;
    g_settings->setArrayIndex(0);
    p.uuid = g_settings->value("uuid").toString();
    p.strName = g_settings->value("name").toString();
    p.strIP = g_settings->value("ip").toString();
    p.nPort = g_settings->value("port").toUInt();
    p.strUser = g_settings->value("user").toString();
    p.strPassword = g_settings->value("password").toString();
    p.nType = (Proxy::ProxyType) g_settings->value("type",0).toInt();
    p.enabled = g_settings->value("enabled").toBool();
    g_settings->endArray();
    return p;
}

Proxy::operator QNetworkProxy() const
{
	QNetworkProxy::ProxyType t;
	
	if(nType == ProxyHttp)
		t = QNetworkProxy::HttpProxy;
	else if(nType == ProxySocks5)
		t = QNetworkProxy::Socks5Proxy;
	else
		t = QNetworkProxy::NoProxy;
	
	QNetworkProxy p(t);
	
	p.setHostName(strIP);
	p.setPort(nPort);
	p.setUser(strUser);
	p.setPassword(strPassword);
	
	return p;
}

void Proxy::saveProxy(int index, QString name,QString ip,QString port, QString user, QString pass, bool enabled) {

    g_settings->beginWriteArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    g_settings->setValue("type", index);
    g_settings->setValue("name", name);
    g_settings->setValue("ip", ip);
    g_settings->setValue("port", port);
    g_settings->setValue("user", user);
    g_settings->setValue("password", pass);
    g_settings->setValue("uuid", "nouuid");
    g_settings->setValue("enabled",enabled);
    g_settings->endArray();
}

QString Proxy::getName(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    QString name = g_settings->value("name").toString();
    g_settings->endArray();
    return name;
}

bool Proxy::isProxyEnabled(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    bool enabled = g_settings->value("enabled").toBool();
    g_settings->endArray();
    return enabled;
}

