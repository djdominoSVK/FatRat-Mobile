#include "SettingsMethods.h"
#include <Queue.h>
#include <QSettings>

extern QSettings* g_settings;
extern QList<Queue*> g_queues;
extern QReadWriteLock g_queuesLock;

SettingsMethods::SettingsMethods(QObject *parent) :
    QObject(parent)
{
}

QString SettingsMethods::getIp(){
   g_settings->beginReadArray("httpftp/proxys");
   g_settings->setArrayIndex(0);
   QString ip = g_settings->value("ip").toString();
   g_settings->endArray();
   return ip;
}

QString SettingsMethods::getPort(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    QString value = g_settings->value("port").toString();
    g_settings->endArray();
    return value;
}

QString SettingsMethods::getUser(){
   g_settings->beginReadArray("httpftp/proxys");
   g_settings->setArrayIndex(0);
   QString value = g_settings->value("user").toString();
   g_settings->endArray();
   return value;
}

QString SettingsMethods::getPassword(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    QString value = g_settings->value("password").toString();
    g_settings->endArray();
    return value;
}
int SettingsMethods::getType(){
    g_settings->beginReadArray("httpftp/proxys");
    g_settings->setArrayIndex(0);
    int value = g_settings->value("type",0).toInt();
    g_settings->endArray();
    return value;
}

int SettingsMethods::getRefresh(){
    return g_settings->value("gui_refresh").toInt();
}
int SettingsMethods::getNetworkSpeedDown(){
    return g_settings->value("network/speed_down").toInt();
}
int SettingsMethods::getNetworkSpeedUp(){
    return g_settings->value("network/speed_up").toInt();
}
int SettingsMethods::getNetworkMaximum(){
    return g_settings->value("network/maximum_transfers").toInt();
}

void SettingsMethods::saveSettings(QString refresh,QString down, QString up, QString maximum){

       g_settings->setValue("gui_refresh", refresh);
       g_settings->setValue("network/speed_down", down );
       g_settings->setValue("network/speed_up", up );

       g_settings->setValue("network/maximum_transfers", maximum);


}
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

void SettingsMethods::saveProxy(int index, QString name,QString ip,QString port, QString user, QString pass, bool enabled) {
    Proxy::saveProxy(index, name,ip,port, user, pass,enabled);
}

//void SettingsMethods::doneQueue(Queue* q, bool unlock)
//{
//    if(q != 0)
//    {
//        if(unlock)
//            q->unlock();
//        g_queuesLock.unlock();
//    }
//}

//Queue* SettingsMethods::getQueue(int index, bool lock)
//{
//    g_queuesLock.lockForRead();

//    if(index < 0 || index >= g_queues.size())
//    {
//        if(index != -1)
//            qDebug() << "MainWindow::getQueue(): Invalid queue requested: " << index;
//        g_queuesLock.unlock();
//        return 0;
//    }

//    Queue* q = g_queues[index];
//    if(lock)
//        q->lock();
//    return q;
//}
