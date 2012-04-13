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

#ifndef NEWTRANSFERDLG_H
#define NEWTRANSFERDLG_H
#include <QWidget>
#include "Proxy.h"
#include "Auth.h"
#include "Queue.h"
#include "Transfer.h"
#include <QDir>

class NewTransferDlg : public QWidget
{
Q_OBJECT

public:
    NewTransferDlg();
    NewTransferDlg(QWidget* parent): QWidget(parent) {}

    Q_INVOKABLE QString addTextFile();
    Q_INVOKABLE QString browse(QString current);
    Q_INVOKABLE QString browse2();
    Q_INVOKABLE void createTransfer(QString m_strURIs,bool downloadTrueUploadFalse,int m_nClass,QString m_strDestination,
                                        int m_nDownLimit,int m_nUpLimit,bool m_bPaused);

    Q_INVOKABLE void authData(QString regExp, QString user, QString pass) {
        m_auth.strRegExp = regExp;
        m_auth.strUser = user;
        m_auth.strPassword = pass;
    }
    Q_INVOKABLE QString authUsername(){
        return m_auth.strUser;
    }
    Q_INVOKABLE QString authPassword(){
        return m_auth.strPassword;
    }

    Q_INVOKABLE void saveProxy(int index, QString name,QString ip,QString port, QString user, QString pass, bool enabled) {
        Proxy::saveProxy(index, name,ip,port, user, pass,enabled);
    }
    Q_INVOKABLE QString getIp();
    Q_INVOKABLE QString getPort();
    Q_INVOKABLE QString getUser();
    Q_INVOKABLE QString getPassword();
    Q_INVOKABLE int getType();

    Q_INVOKABLE QString getProxyName(){
        return Proxy::getName();
    }
    Q_INVOKABLE bool isProxyEnabled(){
        return Proxy::isProxyEnabled();
    }

    Q_INVOKABLE QString getLinksFromFile(QString filename );
    Q_INVOKABLE QString removeFileFromString(QString filename );

    Q_INVOKABLE void saveSettings(QString refresh,QString down, QString up, QString maximum);
    Q_INVOKABLE void initSettings();
    Q_INVOKABLE int getRefresh();
    Q_INVOKABLE int getNetworkSpeedDown();
    Q_INVOKABLE int getNetworkSpeedUp();
    Q_INVOKABLE int getNetworkMaximum();

    Proxy m_proxy;
    Auth m_auth;
protected:
    static Queue* getQueue(int index, bool lock = true);
    void doneQueue(Queue* q, bool unlock = true, bool refresh = true);
private:
    QList<Proxy > m_listProxy;
};



#endif
