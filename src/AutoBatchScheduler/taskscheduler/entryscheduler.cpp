#include "entryscheduler.h"

EntryScheduler::EntryScheduler(QObject *parent) : QObject(parent)
{
    scalc = new SchedulerCalc();

    //set internal thread
    worker = new QThread();

    //connect thread
    connect(worker, &QThread::started, scalc, &SchedulerCalc::start);

    connect(scalc, &SchedulerCalc::timerStarted, this, &EntryScheduler::receiveStarted);
    connect(scalc, &SchedulerCalc::timerFinished, this, &EntryScheduler::receiveFinished);
    connect(scalc, &SchedulerCalc::encounterScheduledTime, this, &EntryScheduler::receiveEncounter);

    scalc->moveToThread(worker);

    connect(this, &EntryScheduler::objectNameChanged, this, &EntryScheduler::setXmlItemId);
}

EntryScheduler::~EntryScheduler()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete scalc;
}

bool EntryScheduler::isStarted()
{
    return scalc->getRunning();
}

void EntryScheduler::setRefreshTime(int sec)
{
    scalc->setRefreshms(sec);
}

//void EntryScheduler::setXmlItemId(int itemid)
//{
//    scalc->setSelectedxmlindex(itemid);
//}

void EntryScheduler::setMutex(QMutex *mutex)
{
    scalc->setMutex(mutex);
}

void EntryScheduler::start()
{
    worker->start();
}

void EntryScheduler::stop()
{
    scalc->loopstop();
    scalc->stop();
//    if(worker->isRunning()){
//        worker->quit();
//        worker->wait();
//    }
}

void EntryScheduler::setXmlItemId(QString objname)
{
    scalc->setSelectedxmlindex(objname);
}
