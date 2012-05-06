#ifndef PLUGINS_H
#define PLUGINS_H



#include <QtNetwork/QNetworkReply>
#include <qeventloop.h>
#include <QtDebug>
#include <QStringList>
#include <QTimer>

class Plugins
{
public:
    Plugins();

    QString checkUrl(QUrl url){
        QString te= url.host();
        qDebug() << url.host();
        QString lalal = te;


        if (url.host().contains("dataport")){
            QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
            QNetworkReply *reply = networkMgr->get( QNetworkRequest( url ) ) ;
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));

            // Execute the event loop here, now we will wait here until readyRead() signal is emitted
            // which in turn will trigger event loop quit.
            loop.exec();
            QString data = reply->readAll();
        //    qDebug()<< data;
            QString link;
            QStringList parse;

            if ((data != NULL) && (data != "")){

                parse = data.split("<td><a href=\"");
                if ((!parse.isEmpty())&&(parse.size()> 1)){
                    link = parse[1];
                    // parse = link.split("<a href=\"");
                    // if ((!parse.isEmpty())&&(parse[1] != NULL)){
                    //    link = parse[1];
                    parse = link.split("\" style=\"color: white;");
                    if ((!parse.isEmpty())&&(parse.size() >= 1)){
                        link = parse[0];
                 //       qDebug() << link;
                        if (link=="http://dataport.cz/kredit/"){

                            return "failed";
                        }
                        else {return link;}
                    }
                    else return "failed";
                    // }
                    //else return NULL;
                }
                else return "failed";
            }
            else {
                return "failed";
            }
        }
        //Mediafire
        //Mediafire
        //Mediafire
        if (url.host().contains("mediafire")){
            QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
            QNetworkReply *reply = networkMgr->get( QNetworkRequest( url ) ) ;
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

            // Execute the event loop here, now we will wait here until readyRead() signal is emitted
            // which in turn will trigger event loop quit.
            loop.exec();
            QString data = reply->readAll();
       //     qDebug()<< data;
            QString link;
            QStringList parse;
            if ((data != NULL) && (data != "")){
                parse = data.split("<div class=\"download_link\" id=");
                if ((!parse.isEmpty())&&(parse.size() > 1)){
                    link = parse[1];
                    parse = link.split("<a href=\"");
                    if ((!parse.isEmpty())&&(parse.size() > 1)){
                        link = parse[1];
                        parse = link.split("\" onclick=");
                        if ((!parse.isEmpty())&&(parse.size() >= 1)){
                            link = parse[0];
                    //        qDebug() << link;
                            return link;
                        }
                        else return "failed";
                    }
                    else return "failed";
                }
                else return "failed";
            }
            else {
                return "failed";
            }
        }
        if (url.host().contains("sendspace")){
            //sendspace
            //sendspace
            //sendspace
            QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
            QNetworkReply *reply = networkMgr->get( QNetworkRequest( url ) ) ;
            QEventLoop loop;

            //connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));

            // Execute the event loop here, now we will wait here until readyRead() signal is emitted
            // which in turn will trigger event loop quit.
            loop.exec();

            QString data = reply->readAll();
            QString link;
            QStringList parse;
            if ((data != NULL) && (data != "")){
                parse = data.split("id=\"download_button\" href=\"");
                if ((!parse.isEmpty())&&(parse.size() > 1)){
                    link = parse[1];
                    parse = link.split("\" onclick=");
                    if ((!parse.isEmpty())&&(parse.size() >= 1)){
                        link = parse[0];
                        qDebug() << link;
                        return link;
                    }
                    else return "failed";

                }
                else return "failed";
            }
            else {
                return "failed";
            }
        }

        return "unknown_server";
    }

};

#endif // PLUGINS_H
