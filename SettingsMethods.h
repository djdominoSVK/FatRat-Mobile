#ifndef SETTINGSMETHODS_H
#define SETTINGSMETHODS_H

#include <QObject>
#include "Proxy.h"

class SettingsMethods : public QObject
{
    Q_OBJECT
public:
    SettingsMethods(QObject *parent = 0);

    // proxy settings
    Q_INVOKABLE QString getIp();
    Q_INVOKABLE QString getPort();
    Q_INVOKABLE QString getUser();
    Q_INVOKABLE QString getPassword();
    Q_INVOKABLE int getType();

    Q_INVOKABLE QString getProxyName();
    Q_INVOKABLE bool isProxyEnabled();
    Q_INVOKABLE void saveProxy(int index, QString name,QString ip,QString port, QString user, QString pass, bool enabled);


    // common settings
    Q_INVOKABLE void saveSettings(QString refresh,QString down, QString up, QString maximum);
    Q_INVOKABLE void initSettings();
    Q_INVOKABLE int getRefresh();
    Q_INVOKABLE int getNetworkSpeedDown();
    Q_INVOKABLE int getNetworkSpeedUp();
    Q_INVOKABLE int getNetworkMaximum();
    
};

#endif // SETTINGSMETHODS_H
