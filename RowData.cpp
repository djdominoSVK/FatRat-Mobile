#include "RowData.h"
#include "Transfer.h"

/**
 * @brief simple constructor
 *
 */
RowData::RowData(): m_name(""), m_speedDown(""), m_speedUp(""), m_timeLeft(""),m_message(""), m_progress(""), m_size(""),m_actSpeed(""),m_destination(""),m_source(""), m_fProgress(0)
{}

/**
 * @brief creating instance of rowdata from transfer
 *
 * @param t transfer which will be inserted into data model
 */
RowData::RowData(Transfer* t) : m_state(t->state()),m_name(t->name()),m_message(t->message()), m_mode(t->mode()), m_primaryMode(t->primaryMode())
{
    const qint64 total = t->total();
    m_fProgress = (total) ? 100.0/t->total()*t->done() : 0;
    if(t->total()){
        m_progress = QString("%1 from %2 (%3%)").arg(formatSize(t->done(),false)).arg(formatSize(t->total(),false)).arg(100.0/t->total()*t->done(), 0, 'f', 1);
        m_size = formatSize(t->total(),false);
    }
    else{
        m_progress = QString("%1, total size unknown").arg(formatSize(t->done(),false));
        m_size = "?";
    }
    if (t->mode() == (Transfer::Upload)){
        m_destination = t->url();
        m_source = t->dataPath(true);
    }
    else{
        m_destination = t->dataPath(false);
        m_source = t->url();
    }

    if(t->isActive())
    {
        int down,up;
        t->speeds(down,up);
        if(down)
        {
            m_actSpeed = QString("%1 kB/s").arg(double(down)/1024.f, 0, 'f', 1);
        }
        if(up)
        {
            m_actSpeed += QString("%1 kB/s").arg(double(up)/1024.f, 0, 'f', 1);

        }

        if(t->total())
        {
            qulonglong totransfer = t->total() - t->done();
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

/**
 * @brief setting state
 *
 * @param state state
 */
void RowData::setState(const Transfer::State state) {
    if (state != m_state) {
        m_state = state;
    }
}

/**
 * @brief getting state
 *
 * @return Transfer::State state
 */
Transfer::State RowData::state() const {
    return m_state;
}

/**
 * @brief setting name
 *
 * @param name name
 */
void RowData::setName(const QString name) {
    if (name != m_name) {
        m_name = name;
    }
}

/**
 * @brief getting name
 *
 * @return QString name
 */
QString RowData::name() const {
    return m_name;
}

/**
 * @brief setting maximum download speed
 *
 * @param speedDown maximum download speed
 */
void RowData::setSpeedDown(const QString speedDown) {
    if (speedDown != m_speedDown) {
        m_speedDown = speedDown;
    }
}

/**
 * @brief getting maximum download speed
 *
 * @return QString maximum download speed
 */
QString RowData::speedDown() const {
    return m_speedDown;
}

/**
 * @brief setting maximum upload speed
 *
 * @param speedUp maximum upload speed
 */
void RowData::setSpeedUp(const QString speedUp) {
    if (speedUp != m_speedUp) {
        m_speedUp = speedUp;
    }
}

/**
 * @brief getting maximum upload speed
 *
 * @return QString maximum upload speed
 */
QString RowData::speedUp() const {
    return m_speedUp;
}

/**
 * @brief setting time left
 *
 * @param timeLeft time left
 */
void RowData::setTimeLeft(const QString timeLeft) {
    if (timeLeft != m_timeLeft) {
        m_timeLeft = timeLeft;
    }
}

/**
 * @brief getting time left
 *
 * @return QString time left
 */
QString RowData::timeLeft() const {
    return m_timeLeft;
}

/**
 * @brief setting message
 *
 * @param message message
 */
void RowData::setMessage(const QString message) {
    if (message != m_message) {
        m_message = message;
    }
}

/**
 * @brief getting message
 *
 * @return QString message
 */
QString RowData::message() const {
    return m_message;
}

/**
 * @brief setting progress
 *
 * @param progress progress
 */
void RowData::setProgress(const QString progress) {
    if (progress != m_progress) {
        m_progress = progress;
    }
}

/**
 * @brief getting progress
 *
 * @return QString size
 */
QString RowData::progress() const {
    return m_progress;
}

/**
 * @brief setting size
 *
 * @param size size
 */
void RowData::setSize(const QString size) {
    if (size != m_size) {
        m_size = size;
    }
}

/**
 * @brief getting size
 *
 * @return QString size
 */
QString RowData::size() const {
    return m_size;
}

/**
 * @brief setting mode
 *
 * @param mode mode
 */
void RowData::setMode(const Transfer::Mode mode) {
    if (mode != m_mode) {
        m_mode = mode;
    }
}

/**
 * @brief getting mode
 *
 * @return Transfer::Mode mode
 */
Transfer::Mode RowData::mode() const {
    return m_mode;
}

/**
 * @brief setting primary mode
 *
 * @param primaryMode primary mode
 */
void RowData::setPrimaryMode(const Transfer::Mode primaryMode) {
    if (primaryMode != m_primaryMode) {
        m_primaryMode = primaryMode;
    }
}

/**
 * @brief getting primary mode
 *
 * @return Transfer::Mode primary mode
 */
Transfer::Mode RowData::primaryMode() const {
    return m_primaryMode;
}

/**
 * @brief setting progress in float
 *
 * @param fProgress progress in float
 */
void RowData::setFProgress(const float fProgress) {
    if (fProgress != m_fProgress) {
        m_fProgress = fProgress;
    }
}

/**
 * @brief getting progress in float
 *
 * @return float progress in float
 */
float RowData::fProgress() const {
    return m_fProgress;
}


/**
 * @brief setting actual speed
 *
 * @return QString actual speed
 */
QString RowData::speed() const {
    return m_actSpeed;
}

/**
 * @brief getting actual speed
 *
 * @param speed actual speed
 */
void RowData::setSpeed(const QString speed) {
    if (speed != m_actSpeed) {
        m_actSpeed = speed;
    }
}

/**
 * @brief setting destiantion of transfer
 *
 * @return QString destiantion of transfer
 */
QString RowData::destination() const {
    return m_destination;
}

/**
 * @brief getting destiantion of transfer
 *
 * @param destination destiantion of transfer
 */
void RowData::setDestination(const QString destination) {
    if (destination != m_destination) {
        m_destination = destination;
    }
}

/**
 * @brief setting source of transfer
 *
 * @return QString source of transfer
 */
QString RowData::source() const {
    return m_source;
}

/**
 * @brief getting source of transfer
 *
 * @param source source of transfer
 */
void RowData::setSource(const QString source) {
    if (source != m_source) {
        m_source = source;
    }
}


/**
 * @brief computing size or speed from amount of transfered data
 *
 * @param size size in bytes
 * @param persec true if output should be speed
 * @return QString formated size
 */
QString RowData::formatSize(qulonglong size, bool persec)
{
    QString rval;

    if(size < 1024)
        rval = QString("%L1 B").arg(size);
    else if(size < 1024*1024)
        rval = QString("%L1 KB").arg(size/1024);
    else if(size < 1024*1024*1024)
        rval = QString("%L1 MB").arg(double(size)/1024.0/1024.0, 0, 'f', 1);
    else
        rval = QString("%L1 GB").arg(double(size)/1024.0/1024.0/1024.0, 0, 'f', 1);

    if(persec) rval += "/s";
    return rval;
}

/**
 * @brief formating time
 *
 * @param inval time interval
 * @return QString returns converted time
 */
QString RowData::formatTime(qulonglong inval)
{
    QString result;
    qulonglong days,hrs,mins,secs;
    days = inval/(60*60*24);
    inval %= 60*60*24;

    hrs = inval/(60*60);
    inval %= 60*60;

    mins = inval/60;
    secs = inval%60;

    if(days)
        result = QString("%1d ").arg(days);
    if(hrs)
        result += QString("%1h ").arg(hrs);
    if(mins)
        result += QString("%1m ").arg(mins);
    if(secs)
        result += QString("%1s").arg(secs);

    return result;
}
