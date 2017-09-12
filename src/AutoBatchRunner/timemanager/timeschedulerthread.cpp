#include "timeschedulerthread.h"

#include <QMutex>

TimeSchedulerThread::TimeSchedulerThread(QObject *parent) : QObject(parent)
{
    scalc = new SchedulerCalc();
    builder = new StartupXmlBuilder();

    //set internal thread
    workerthread = new QThread();

    //connect thread
    connect(workerthread, &QThread::started, this, &TimeSchedulerThread::doWork);

    refreshtime = 1;
    xmlitemid = -1;
    name = "";
}

TimeSchedulerThread::~TimeSchedulerThread()
{
    if(workerthread->isRunning()){
        workerthread->quit();
        workerthread->wait();
    }
    delete scalc;
    delete builder;
    delete workerthread;
}

bool TimeSchedulerThread::isStarted()
{
    return running;
}

void TimeSchedulerThread::setMovedThread(QThread *thread)
{
    workerthread = thread;
}

void TimeSchedulerThread::setThreadName(QString name)
{
    this->name = name;
}

void TimeSchedulerThread::setRefreshTime(int sec)
{
    this->refreshtime = sec;
}

void TimeSchedulerThread::setScheduledTime(QDateTime scheduled)
{
    this->scheduled = scheduled;
}

void TimeSchedulerThread::setXmlItemId(int itemid)
{
    xmlitemid = itemid;
}

void TimeSchedulerThread::forceStop()
{
    if(workerthread->isRunning()){
        running = false;
        workerthread->quit();
        //force stopped thread
        emit timerAlreadyTimeout(name);
    }
}

void TimeSchedulerThread::stop()
{
    if(workerthread->isRunning()){
        running = false;
        workerthread->quit();
    }
}

QString TimeSchedulerThread::getName()
{
    return name;
}

QDateTime TimeSchedulerThread::getScheduledTime()
{
    return scheduled;
}

int TimeSchedulerThread::getXmlId()
{
    return xmlitemid;
}

QThread *TimeSchedulerThread::getMovedThread()
{
    return workerthread;
}

void TimeSchedulerThread::start()
{
    this->moveToThread(workerthread);
    workerthread->start();
}

void TimeSchedulerThread::doWork()
{
    qint64 lesttime;
    QMutex mutex;
    //check null
    if(xmlitemid == -1) return;
    if(name == "") return;

    QDateTime scheduledDateTime;

    mutex.lock();

    //schedule instructed
    qDebug() << "::scheduled_name::" << name;
    if(scheduled.isValid()){
        scheduledDateTime = scheduled;
    }else{
        scheduledDateTime = scalc->init_GetScheduledTimeFromXml(xmlitemid);
        scheduled = scheduledDateTime;
    }

    qDebug() << scheduledDateTime << "TimeSchedulerThread::start()";

    //stop timer when first time expired.
    if(scalc->getLestSeconds(scheduledDateTime) < 0){
        qDebug() << "TimeSchedulerThread::start() : timerStopped()";
        forceStop();
        return;
    }

    //for restarting
    running = true;

    emit timerStarted(name);

    while(running){
        lesttime = scalc->getLestSeconds(scheduledDateTime);
        if(lesttime > 0){
            QThread::sleep(refreshtime);
        }else{
            emit encounteredTimeout(name);
            return;
        }
    }
    mutex.unlock();
}
