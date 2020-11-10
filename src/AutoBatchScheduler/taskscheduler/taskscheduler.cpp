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

#include "taskscheduler.h"

TaskScheduler::TaskScheduler(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, EntryTask *>();
    scheduler = new QHash<QString, EntryScheduler *>();
    basemutex = new QMutex();
}

TaskScheduler::~TaskScheduler()
{
    delete task;
    delete scheduler;
    delete basemutex;
}

bool TaskScheduler::taskRunningCheck(QString objectname)
{
    bool result = false;
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        result = et->getStarted();
    }

    return result;
}

bool TaskScheduler::processAliveCheck(QString objectname)
{
    return task->contains(objectname) && scheduler->contains(objectname) ? true : false;
}

void TaskScheduler::addTask(const SchedulerCache &sc)
{
    //new task set
    EntryTask *et = new EntryTask();
    EntryScheduler *es = new EntryScheduler();

    //connect child object
    Executor *ec = et->getExecutor();
    connect(ec, &Executor::processStateCount, this, &TaskScheduler::receiveInitCount);
    connect(ec, &Executor::processStateUpdate, this, &TaskScheduler::receiveCurrent);
    connect(ec, &Executor::processCheckError, this, &TaskScheduler::receiveErrorText);
    connect(ec, &Executor::processMessage, this, &TaskScheduler::receiveMessage);

    connect(ec, &Executor::processStarted, this, &TaskScheduler::receiveStarted);
    connect(ec, &Executor::processPaused, this, &TaskScheduler::receivePaused);
    connect(ec, &Executor::processStopped, this, &TaskScheduler::receiveStopped);
    connect(ec, &Executor::processEnded, this, &TaskScheduler::receiveEnd);

    SchedulerWait *sw = es->getSchedulerWait();
    connect(sw, &SchedulerWait::timerStarted, this, &TaskScheduler::receiveTimerStarted);
    connect(sw, &SchedulerWait::timerFinished, this, &TaskScheduler::receiveTimerFinished);
    connect(sw, &SchedulerWait::encounterScheduledTime, this, &TaskScheduler::receiveEncounter);

    //set task filename
    et->setFile(sc.profilePath);

    //set wait information
    sw->setSchedulerCache(sc);

    //set same objectname
    et->setObjectName(sc.objectName());
    ec->setObjectName(sc.objectName());
    es->setObjectName(sc.objectName());
    sw->setObjectName(sc.objectName());

    //set shared mutex
    es->setMutex(basemutex);

    //TODO:refreshtime in scheduler

    //add pointer
    task->insert(sc.objectName(), et);
    scheduler->insert(sc.objectName(), es);

    //start scheduler
    scheduler->value(sc.objectName())->start();
}

void TaskScheduler::removeTask(QString objectname)
{
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        EntryScheduler *es = scheduler->value(objectname);

        //stop process
        et->stop();
        es->stop();

        const Executor *ec = et->getExecutor();
        disconnect(ec, &Executor::processStateCount, this, &TaskScheduler::receiveInitCount);
        disconnect(ec, &Executor::processStateUpdate, this, &TaskScheduler::receiveCurrent);
        disconnect(ec, &Executor::processCheckError, this, &TaskScheduler::receiveErrorText);
        disconnect(ec, &Executor::processMessage, this, &TaskScheduler::receiveMessage);

        disconnect(ec, &Executor::processStarted, this, &TaskScheduler::receiveStarted);
        disconnect(ec, &Executor::processPaused, this, &TaskScheduler::receivePaused);
        disconnect(ec, &Executor::processStopped, this, &TaskScheduler::receiveStopped);
        disconnect(ec, &Executor::processEnded, this, &TaskScheduler::receiveEnd);

        const SchedulerWait *sw = es->getSchedulerWait();
        disconnect(sw, &SchedulerWait::timerStarted, this, &TaskScheduler::receiveTimerStarted);
        disconnect(sw, &SchedulerWait::timerFinished, this, &TaskScheduler::receiveTimerFinished);
        disconnect(sw, &SchedulerWait::encounterScheduledTime, this, &TaskScheduler::receiveEncounter);

        delete et;
        delete es;

        //delete resister
        task->remove(objectname);
        scheduler->remove(objectname);
    }
}

void TaskScheduler::processPause(QString objectname)
{
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        if(et->getStarted() && !et->getPause()){
            et->pause(true);
        }else{
            et->pause(false);
        }
    }
}

void TaskScheduler::processStop(QString objectname)
{
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        if(et->getStarted()){
            et->stop();
        }
    }
}

void TaskScheduler::sendInput(QString objectname, QString text)
{
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        if(et->getStarted()){
            et->sendInput(text);
        }
    }
}

void TaskScheduler::receiveTimerFinished(int value)
{
    //if expired, remove from list
    // objectname deleted
    QString sendfrom = sender()->objectName();
    if(value  == SchedulerWait::EXPIRED){
        removeTask(sendfrom);
    }
    emit timerFinished(sendfrom, value);
}

void TaskScheduler::receiveEncounter()
{
    QString objname = sender()->objectName();
    if(task->contains(objname)){
        task->value(objname)->start();
        emit encounteredScheduledTime(objname);
    }
}
