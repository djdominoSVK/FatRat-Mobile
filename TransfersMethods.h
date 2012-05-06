#ifndef TRANSFERSMETHODS_H
#define TRANSFERSMETHODS_H

#include <QObject>
#include <Auth.h>

class TransfersMethods : public QObject
{
    Q_OBJECT
public:
    TransfersMethods(QObject *parent = 0);
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

    Q_INVOKABLE QString getLinksFromFile(QString filename );
    Q_INVOKABLE QString removeFileFromString(QString filename );
    
private:
    Auth m_auth;
    
};

#endif // TRANSFERSMETHODS_H
