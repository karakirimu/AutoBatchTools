/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

void EntryScheduler::setRefreshTime(ulong milliseconds)
{
    swait->setRefreshms(milliseconds);
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
