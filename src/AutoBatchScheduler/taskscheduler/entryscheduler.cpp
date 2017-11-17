#include "entryscheduler.h"

EntryScheduler::EntryScheduler(QObject *parent) : QObject(parent)
{
    swait = new SchedulerWait();

    //set internal thread
    worker = new QThread();

    //connect thread
    connect(worker, &QThread::started, swait, &SchedulerWait::start);

    connect(swait, &SchedulerWait::timerStarted, this, &EntryScheduler::receiveStarted);
    connect(swait, &SchedulerWait::timerFinished, this, &EntryScheduler::receiveFinished);
    connect(swait, &SchedulerWait::encounterScheduledTime, this, &EntryScheduler::receiveEncounter);

    swait->moveToThread(worker);

    connect(this, &EntryScheduler::objectNameChanged, this, &EntryScheduler::setXmlItemId);
}

EntryScheduler::~EntryScheduler()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete swait;
}

bool EntryScheduler::isStarted()
{
    return swait->getRunning();
}

void EntryScheduler::setRefreshTime(int sec)
{
    swait->setRefreshms(sec);
}

//void EntryScheduler::setXmlItemId(int itemid)
//{
//    scalc->setSelectedxmlindex(itemid);
//}

void EntryScheduler::setMutex(QMutex *mutex)
{
    swait->setMutex(mutex);
}

void EntryScheduler::start()
{
    worker->start();
}

void EntryScheduler::stop()
{
    swait->loopstop();
    swait->stop();
//    if(worker->isRunning()){
//        worker->quit();
//        worker->wait();
//    }
}

void EntryScheduler::setXmlItemId(QString objname)
{
    swait->setSelectedxmlindex(objname);
}
