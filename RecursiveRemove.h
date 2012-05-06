#ifndef RECURSIVEREMOVE_H
#define RECURSIVEREMOVE_H

#include <QThread>
#include <QString>



class RecursiveRemove : public QThread
{
public:
    RecursiveRemove(QString what);
    void run()
    {
        connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
        work(m_what);
    }
    static bool work(QString what)
    {
        qDebug() << "recursiveRemove" << what;
        if(!QFile::exists(what))
            return true; // silently ignore
        if(!QFile::remove(what))
        {
            QDir dir(what);
            if(!dir.exists())
            {
                qDebug() << "Not a directory:" << what;
                return false;
            }

            QStringList contents;
            contents = dir.entryList();

            foreach(QString item, contents)
            {
                if(item != "." && item != "..")
                {
                    if(!work(dir.filePath(item)))
                        return false;
                }
            }

            QString name = dir.dirName();
            if(!dir.cdUp())
            {
                qDebug() << "Cannot cdUp:" << what;
                return false;
            }
            if(!dir.rmdir(name))
            {
                qDebug() << "Cannot rmdir:" << name;
                return false;
            }
        }
        return true;
    }
private:
    QString m_what;
};

#endif // RECURSIVEREMOVE_H
