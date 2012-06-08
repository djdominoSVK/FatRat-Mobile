#include "Plugins.h"


#include <QtNetwork/QNetworkReply>
#include <qeventloop.h>
#include <QtDebug>
#include <QStringList>
#include <QTimer>



/**
 * @brief chceck url for filesharing servers, if server is known returns real url of file
 *
 * @param url source ulr
 * @return QString new ulr or "failed" if problem or "unknown_server" if server is unknown
 */
QString Plugins::checkUrl(QUrl url){


//        DATAPORT CHANGED THEIR DESIGN AND ITS CURRENTLY NOT WORKING
//        if (url.host().contains("dataport")){
//            QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
//            QNetworkReply *reply = networkMgr->get( QNetworkRequest( url ) ) ;
//            QEventLoop loop;
//            QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
//            loop.exec();
//            QString data = reply->readAll();
//            QString link;
//            QStringList parse;
//            if ((data != NULL) && (data != "")){
//                parse = data.split("<td><a href=\"");
//                if ((!parse.isEmpty())&&(parse.size()> 1)){
//                    link = parse[1];
//                    parse = link.split("\" style=\"color: white;");
//                    if ((!parse.isEmpty())&&(parse.size() >= 1)){
//                        link = parse[0];
//                        if (link=="http://dataport.cz/kredit/"){
//                            return "failed";
//                        }
//                        else {return link;}
//                    }
//                    else return "failed";
//                }
//                else return "failed";
//            }
//            else {
//                return "failed";
//            }
//        }



        //Mediafire
        //Mediafire
        //Mediafire
        if (url.host().contains("mediafire")){
            QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
            QNetworkReply *reply = networkMgr->get( QNetworkRequest( url ) ) ;
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            QString data = reply->readAll();
            QString link;
            QStringList parse;
            if ((data != NULL) && (data != "")){
                parse = data.split("download_link");
                if ((!parse.isEmpty())&&(parse.size() > 1)){
                    link = parse[parse.size()-1];
                    parse = link.split("http://");
                    if ((!parse.isEmpty())&&(parse.size() > 1)){
                        link = parse[1];
                        parse = link.split("\";");
                        if ((!parse.isEmpty())&&(parse.size() >= 1)){
                            link = "http://" + parse[0];
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
            QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
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

