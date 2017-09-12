#include "timemanager.h"

TimeManager::TimeManager(QObject *parent) : QObject(parent)
{
    timerlist = new QList<TimeSchedulerThread*>();
    scalc = new SchedulerCalc();
}

TimeManager::~TimeManager()
{
    delete timerlist;
    delete scalc;
}

QString TimeManager::addTimer(int itemid)
{
    qDebug() << "TimeManager::addTimer(int itemid) : (add timer)";
    TimeSchedulerThread *tsth = new TimeSchedulerThread();
    QString threadobjname = getRandomString(32);

    //set baseid
    tsth->setXmlItemId(itemid);
    tsth->setThreadName(threadobjname);

    //timemanager connect
    connect(tsth, &TimeSchedulerThread::timerStarted, this, &TimeManager::threadStarted);
    connect(tsth, &TimeSchedulerThread::timerStopped, this, &TimeManager::threadStopped);
    connect(tsth, &TimeSchedulerThread::encounteredTimeout, this, &TimeManager::loadExec);
    connect(tsth, &TimeSchedulerThread::timerAlreadyTimeout, this, &TimeManager::threadObsoluted);

    //store manage data
    timerlist->append(tsth);

    //run thread
    tsth->start();

    return threadobjname;
}

void TimeManager::removeTimer(QString name)
{
    qDebug() << "TimeManager::removeTimer(QString name) : (remove timer)";
    for(int i = 0; i < timerlist->count(); i++){
        if(timerlist->at(i)->getName() == name && timerlist->at(i)->isStarted()){

            TimeSchedulerThread *tstd = timerlist->at(i);
            timerlist->removeAt(i);

            tstd->stop();
            delete tstd;          
            if(i > 0) i--;
        }
    }
}

QDateTime TimeManager::getScheduledTime(QString threadname)
{
//    qDebug() << "TimeManager::getScheduledTime(QString threadname)";
    int count = timerlist->count();
    for(int i = 0; i < count; i++){
        if(timerlist->at(i)->getName() == threadname && timerlist->at(i)->isStarted()){

            TimeSchedulerThread *tstd = timerlist->at(i);
            return tstd->getScheduledTime();
        }
    }

    //load current time
    return QDateTime::currentDateTime();
}

void TimeManager::reloadTimer(TimeSchedulerThread *old)
{
    qDebug() << "TimeManager::reloadTimer(QString name) : (reload timer)";
    TimeSchedulerThread *tsth = old;

    tsth->stop();
    tsth->setScheduledTime(scalc->getNextTimeFromXml(tsth->getXmlId(), tsth->getScheduledTime()));
    tsth->start();
}

void TimeManager::threadStarted(QString name)
{
    //for systemtray only
    emit timerStarted(name);
}

void TimeManager::threadStopped(QString name)
{
    //for systemtray only
    emit timerStopped(name);
}

void TimeManager::threadObsoluted(QString name)
{
    //for systemtray
    emit timerAlreadyExpired(name);
}

void TimeManager::loadExec(QString name)
{
    //load runthread from timescheduler
    emit timerExpired(name);

    //restart timer if possible
    qDebug() << "TimeManager::loadExec(QString name) : (reload timer) :" << name;
    int count = timerlist->count();

    for(int i = 0; i < count; i++){
        qDebug() << "Thread-ALIVE : name :" << timerlist->at(i)->getName() << "running :" << timerlist->at(i)->isStarted();
    }

    for(int i = 0; i < count; i++){
        if(timerlist->at(i)->getName() == name && timerlist->at(i)->isStarted()){

            TimeSchedulerThread *tstd = timerlist->at(i);

            // get radiobutton position
            StartupXmlBuilder builder;
            QList<QStringList> *list = new QList<QStringList>();
            if(builder.readItem(tstd->getXmlId(), list)){

                //table 0 is one shot
                QString selectedradio = list->at(3).at(1);

                //table 1 is week scheduled, table 2 is time scheduled
                if(selectedradio.toInt() > 0) reloadTimer(tstd);

            }
            delete list;
        }
    }
}

QString TimeManager::getRandomString(int size)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;

    for(int i=0; i < size; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
