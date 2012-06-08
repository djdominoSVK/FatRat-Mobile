#include "SettingsMethods.h"
#include <Queue.h>
#include <QSettings>

extern QSettings* g_settings;
extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;

/**
 * @brief simple constructor
 *
 * @param parent
 */
SettingsMethods::SettingsMethods(QObject *parent) :
    QObject(parent)
{
}

/**
 * @brief getting proxy server ip
 *
 * @return QString proxy server ip
 */
QString SettingsMethods::getIp(){
   g_settings->beginReadArray("httpftp/proxys");
   g_settings->setArrayIndex(0);
   QString ip = g_settings->value("ip").toString();
   g_settings->endArray();
   return ip;
}

/**
 * @brief getting proxy server port
 *
 * @return QString proxy server port
 */
QString SettingsMethods::getPort(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    QString value = g_settings->value("port").toString();
    g_settings->endArray();
    return value;
}

/**
 * @brief getting proxy server user
 *
 * @return QString proxy server user
 */
QString SettingsMethods::getUser(){
   g_settings->beginReadArray("httpftp/proxys");
   g_settings->setArrayIndex(0);
   QString value = g_settings->value("user").toString();
   g_settings->endArray();
   return value;
}

/**
 * @brief getting proxy server password
 *
 * @return QString proxy server password
 */
QString SettingsMethods::getPassword(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    QString value = g_settings->value("password").toString();
    g_settings->endArray();
    return value;
}
/**
 * @brief getting proxy server type
 *
 * @return int proxy server type
 */
int SettingsMethods::getType(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    int value = g_settings->value("type",0).toInt();
    g_settings->endArray();
    return value;
}

/**
 * @brief getting refresh frequency
 *
 * @return int refresh frequency
 */
int SettingsMethods::getRefresh(){
    return g_settings->value("gui_refresh").toInt();
}
/**
 * @brief getting maximum download speed of network
 *
 * @return int maximum download speed of network
 */
int SettingsMethods::getNetworkSpeedDown(){
    return g_settings->value("network/speed_down").toInt();
}
/**
 * @brief getting maximum upload speed of network
 *
 * @return int maximum upload speed of network
 */
int SettingsMethods::getNetworkSpeedUp(){
    return g_settings->value("network/speed_up").toInt();
}
/**
 * @brief getting maximum transfers of network
 *
 * @return int maximum transfers of network
 */
int SettingsMethods::getNetworkMaximum(){
    return g_settings->value("network/maximum_transfers").toInt();
}

/**
 * @brief setting up general settings
 *
 * @param refresh refresh frequency
 * @param down maximum download speed of network
 * @param up maximum upload speed of network
 * @param maximum maximum transfers of network
 */
void SettingsMethods::saveSettings(QString refresh,QString down, QString up, QString maximum){

       g_settings->setValue("gui_refresh", refresh);
       g_settings->setValue("network/speed_down", down );
       g_settings->setValue("network/speed_up", up );
       g_settings->setValue("network/maximum_transfers", maximum);
}

/**
 * @brief loading general settings, if first time initialize
 *
 */
void SettingsMethods::initSettings(){
   int refresh = g_settings->value("gui_refresh").toInt();
   int speed_down = g_settings->value("network/speed_down").toInt();
   int speed_up = g_settings->value("network/speed_up").toInt();
   int maximum_transfers = g_settings->value("network/maximum_transfers").toInt();

   if (refresh<1){
       g_settings->setValue("gui_refresh", 1);
       refresh = 1;
   }
   if (speed_down<1){
       g_settings->setValue("network/speed_down", 0);
       speed_down=0;
   }
   if (speed_up<1){
       g_settings->setValue("network/speed_up", 0);
       speed_up=0;
   }
   if (maximum_transfers<1){
       g_settings->setValue("network/maximum_transfers", -1);
       maximum_transfers=-1;
   }
   g_queuesLock.lockForRead();
   Queue* q  = g_queues[0];
   if(q){
        q->setSpeedLimits(speed_down * 1024,speed_up * 1024);
        q->setTransferLimits(maximum_transfers,-1);
        q->setUpAsDown(true);
        }
   g_queuesLock.unlock();
}

/**
 * @brief saving proxy settings
 *
 * @param index index
 * @param name name
 * @param ip ip
 * @param port port
 * @param user user
 * @param pass password
 * @param enabled true if enabled
 */
void SettingsMethods::saveProxy(int index, QString name,QString ip,QString port, QString user, QString pass, bool enabled) {
    Proxy::saveProxy(index, name,ip,port, user, pass,enabled);
}

/**
 * @brief getting proxy name
 *
 * @return QString
 */
QString SettingsMethods::getProxyName(){
    return Proxy::getName();
}

/**
 * @brief returns true if proxy is enabled
 *
 * @return bool
 */
bool SettingsMethods::isProxyEnabled(){
    return Proxy::isProxyEnabled();
}
