#ifndef ROWDATA_H
#define ROWDATA_H

#include <QDeclarativeItem>
#include <QItemDelegate>
#include "Transfer.h"

class RowData
{

public:
    RowData(): m_name(""), m_speedDown(""), m_speedUp(""), m_timeLeft(""),m_message(""), m_progress(""), m_size(""), m_fProgress(0),m_actSpeed("")
    {}

    RowData(Transfer* t) : m_state(t->state()),m_name(t->name()),m_message(t->message()), m_mode(t->mode()), m_primaryMode(t->primaryMode())
    {
        const qint64 total = t->total();
        m_fProgress = (total) ? 100.0/t->total()*t->done() : 0;
       // m_progress = (total) ? QString("%1%").arg(m_fProgress, 0, 'f', 1) : QString();

        if(t->total())
            m_progress = QString("%1 from %2 (%3%)").arg(formatSize(t->done(),false)).arg(formatSize(t->total(),false)).arg(100.0/t->total()*t->done(), 0, 'f', 1);
        else
            m_progress = QString("%1, total size unknown").arg(formatSize(t->done(),false));


        m_size = (total) ? formatSize(t->total(),false) : QString("?");

        if(t->isActive())
        {
            int down,up;
            t->speeds(down,up);
            //Transfer::Mode mode = t->primaryMode();
            //QString speed;

            if(down)
                m_actSpeed = QString("%1 kB/s").arg(double(down)/1024.f, 0, 'f', 1);
            if(up)
                m_actSpeed += QString("%1 kB/s").arg(double(up)/1024.f, 0, 'f', 1);

            if(t->total())
            {
                //QString s;
                qulonglong totransfer = t->total() - t->done();

                //if(down && mode == Transfer::Download)
                //    m_progress += QString(tr(", %1 left")).arg( formatTime(totransfer/down) );
                //else if(up && mode == Transfer::Upload)
                //    m_progress += QString(tr(", %1 left")).arg( formatTime(totransfer/up) );
                if(t->primaryMode() == Transfer::Download)
                {
                    if(down)
                        m_timeLeft = formatTime(totransfer/down);
                }
                else if(up)
                    m_timeLeft = formatTime(totransfer/up);
            }

        }
        else
            m_actSpeed=="";

    }

    ~RowData(){}

    Q_INVOKABLE void setState(const Transfer::State state);
    Q_INVOKABLE Transfer::State state() const ;
    //Q_PROPERTY(Transfer::State state READ state WRITE setState NOTIFY stateChanged)

    Q_INVOKABLE void setName(const QString name);
    Q_INVOKABLE QString name() const;
    //Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_INVOKABLE void setSpeedDown(const QString speedDown) ;
    Q_INVOKABLE QString speedDown() const ;
    //Q_PROPERTY(QString speedDown READ speedDown WRITE setSpeedDown NOTIFY speedDownChanged)

    Q_INVOKABLE void setSpeedUp(const QString speedUp) ;
    Q_INVOKABLE QString speedUp() const;
    //Q_PROPERTY(QString speedUp READ speedUp WRITE setSpeedUp NOTIFY speedUpChanged)

    Q_INVOKABLE void setTimeLeft(const QString timeLeft) ;
    Q_INVOKABLE QString timeLeft() const ;
    //Q_PROPERTY(QString timeLeft READ timeLeft WRITE setTimeLeft NOTIFY timeLeftChanged)

    Q_INVOKABLE void setMessage(const QString message) ;
    Q_INVOKABLE QString message() const ;
    //Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

    Q_INVOKABLE void setProgress(const QString progress) ;
     QString progress() const ;
    //Q_PROPERTY(QString progress READ progress WRITE setProgress NOTIFY progressChanged)

    Q_INVOKABLE void setSize(const QString size) ;
    Q_INVOKABLE QString size() const ;
    //Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)

    Q_INVOKABLE void setMode(const Transfer::Mode mode) ;
    Q_INVOKABLE Transfer::Mode mode() const ;
    //Q_PROPERTY(Transfer::Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_INVOKABLE void setPrimaryMode(const Transfer::Mode primaryMode) ;
    Q_INVOKABLE Transfer::Mode primaryMode() const ;
    //Q_PROPERTY(Transfer::Mode primaryMode READ primaryMode WRITE setPrimaryMode NOTIFY primaryModeChanged)

    Q_INVOKABLE void setFProgress(const float fProgress) ;
    Q_INVOKABLE float fProgress() const ;
    //Q_PROPERTY(float fProgress READ fProgress WRITE setFProgress NOTIFY fProgressChanged)


    Q_INVOKABLE void setSpeed(const QString speed) ;
    Q_INVOKABLE QString speed() const;

    static RowData createDataSet(Transfer* t);
    QString formatTime(qulonglong inval);
    QString formatSize(qulonglong size, bool persec);


//signals:
//    void stateChanged();
//    void nameChanged();
//    void speedDownChanged();
//    void speedUpChanged();
//    void timeLeftChanged();
//    void messageChanged();
//    void progressChanged();
//    void sizeChanged();
//    void modeChanged();
//    void primaryModeChanged();
//    void fProgressChanged();

private:
        Transfer::State m_state;
        QString m_name, m_speedDown, m_speedUp, m_timeLeft, m_message, m_progress, m_size, m_actSpeed;
        Transfer::Mode m_mode, m_primaryMode;
        float m_fProgress;


};

#endif // ROWDATA_H
