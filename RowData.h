#ifndef ROWDATA_H
#define ROWDATA_H

#include <QDeclarativeItem>
#include <QItemDelegate>
#include "Transfer.h"

class RowData
{

public:
    RowData();
    RowData(Transfer* t);

    ~RowData(){}

    Q_INVOKABLE void setState(const Transfer::State state);
    Q_INVOKABLE Transfer::State state() const ;

    Q_INVOKABLE void setName(const QString name);
    Q_INVOKABLE QString name() const;

    Q_INVOKABLE void setSpeedDown(const QString speedDown) ;
    Q_INVOKABLE QString speedDown() const ;

    Q_INVOKABLE void setSpeedUp(const QString speedUp) ;
    Q_INVOKABLE QString speedUp() const;

    Q_INVOKABLE void setTimeLeft(const QString timeLeft) ;
    Q_INVOKABLE QString timeLeft() const ;

    Q_INVOKABLE void setMessage(const QString message) ;
    Q_INVOKABLE QString message() const ;

    Q_INVOKABLE void setProgress(const QString progress) ;
    Q_INVOKABLE QString progress() const ;

    Q_INVOKABLE void setSize(const QString size) ;
    Q_INVOKABLE QString size() const ;

    Q_INVOKABLE void setMode(const Transfer::Mode mode) ;
    Q_INVOKABLE Transfer::Mode mode() const ;

    Q_INVOKABLE void setPrimaryMode(const Transfer::Mode primaryMode) ;
    Q_INVOKABLE Transfer::Mode primaryMode() const ;

    Q_INVOKABLE void setFProgress(const float fProgress) ;
    Q_INVOKABLE float fProgress() const ;

    Q_INVOKABLE void setSpeed(const QString speed) ;
    Q_INVOKABLE QString speed() const;

    Q_INVOKABLE QString destination() const;
    Q_INVOKABLE void setDestination(const QString destination);

    Q_INVOKABLE QString source() const;
    Q_INVOKABLE void setSource(const QString source);

    QString formatTime(qulonglong inval);
    QString formatSize(qulonglong size, bool persec);


private:
        Transfer::State m_state;
        QString m_name, m_speedDown, m_speedUp, m_timeLeft, m_message, m_progress, m_size, m_actSpeed, m_destination,m_source;
        Transfer::Mode m_mode, m_primaryMode;
        float m_fProgress;


};

#endif // ROWDATA_H
