/*
 * Copyright 2016-2019 karakirimu
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
//    seed = generateRundomName(SEEDLENGTH);
}

TaskScheduler::~TaskScheduler()
{
    //delete all left tasks
    QHash<QString, EntryScheduler *>::iterator i = scheduler->begin();
    while (i != scheduler->end()) {
        removeTask(i.key());
        ++i;
    }

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

void TaskScheduler::addTask(QString objectname, QString processfile)
{
    //new task set
    EntryTask *et = new EntryTask();
    EntryScheduler *es = new EntryScheduler();

    //connect child object
//    connect(et, &EntryTask::processInitCount, this, &TaskScheduler::receiveInitCount);
//    connect(et, &EntryTask::processCurrent, this, &TaskScheduler::receiveCurrent);
//    connect(et, &EntryTask::processError, this, &TaskScheduler::receiveError);
//    connect(et, &EntryTask::processErrorText, this, &TaskScheduler::receiveErrorText);
//    connect(et, &EntryTask::processMessage, this, &TaskScheduler::receiveMessage);
//    connect(et, &EntryTask::processStarted, this, &TaskScheduler::receiveStarted);
//    connect(et, &EntryTask::processPaused, this, &TaskScheduler::receivePaused);
//    connect(et, &EntryTask::processStopped, this, &TaskScheduler::receiveStopped);
//    connect(et, &EntryTask::processEnd, this, &TaskScheduler::receiveEnd);
    Executor *ec = et->getExecutor();
    connect(ec, &Executor::processStateCount, this, &TaskScheduler::receiveInitCount);
    connect(ec, &Executor::processStateUpdate, this, &TaskScheduler::receiveCurrent);
    connect(ec, &Executor::processCheckError, this, &TaskScheduler::receiveErrorText);
    connect(ec, &Executor::processMessage, this, &TaskScheduler::receiveMessage);

    connect(ec, &Executor::processStarted, this, &TaskScheduler::receiveStarted);
    connect(ec, &Executor::processPaused, this, &TaskScheduler::receivePaused);
    connect(ec, &Executor::processStopped, this, &TaskScheduler::receiveStopped);
    connect(ec, &Executor::processEnded, this, &TaskScheduler::receiveEnd);


//    connect(es, &EntryScheduler::timerStarted, this, &TaskScheduler::receiveTimerStarted);
//    connect(es, &EntryScheduler::timerFinished, this, &TaskScheduler::receiveTimerFinished);
//    connect(es, &EntryScheduler::encounteredScheduledTime, this, &TaskScheduler::receiveEncounter);
    SchedulerWait *sw = es->getSchedulerWait();
    connect(sw, &SchedulerWait::timerStarted, this, &TaskScheduler::receiveTimerStarted);
    connect(sw, &SchedulerWait::timerFinished, this, &TaskScheduler::receiveTimerFinished);
    connect(sw, &SchedulerWait::encounterScheduledTime, this, &TaskScheduler::receiveEncounter);

    //set task filename
    et->setFile(processfile);

    //set same objectname
    et->setObjectName(objectname);
    ec->setObjectName(objectname);
    es->setObjectName(objectname);
    sw->setObjectName(objectname);

    //set shared mutex
    es->setMutex(basemutex);

    //TODO:refreshtime in scheduler

    //add pointer
    task->insert(objectname, et);
    scheduler->insert(objectname, es);

    //start scheduler
    scheduler->value(objectname)->start();
}

void TaskScheduler::removeTask(QString objectname)
{
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        EntryScheduler *es = scheduler->value(objectname);

        //stop process
        et->stop();
        es->stop();

        //disconnect child object
//        disconnect(et, &EntryTask::processInitCount, this, &TaskScheduler::receiveInitCount);
//        disconnect(et, &EntryTask::processCurrent, this, &TaskScheduler::receiveCurrent);
//        disconnect(et, &EntryTask::processError, this, &TaskScheduler::receiveError);
//        disconnect(et, &EntryTask::processErrorText, this, &TaskScheduler::receiveErrorText);
//        disconnect(et, &EntryTask::processMessage, this, &TaskScheduler::receiveMessage);
//        disconnect(et, &EntryTask::processStarted, this, &TaskScheduler::receiveStarted);
//        disconnect(et, &EntryTask::processPaused, this, &TaskScheduler::receivePaused);
//        disconnect(et, &EntryTask::processStopped, this, &TaskScheduler::receiveStopped);
//        disconnect(et, &EntryTask::processEnd, this, &TaskScheduler::receiveEnd);

        const Executor *ec = et->getExecutor();
        disconnect(ec, &Executor::processStateCount, this, &TaskScheduler::receiveInitCount);
        disconnect(ec, &Executor::processStateUpdate, this, &TaskScheduler::receiveCurrent);
        disconnect(ec, &Executor::processCheckError, this, &TaskScheduler::receiveErrorText);
        disconnect(ec, &Executor::processMessage, this, &TaskScheduler::receiveMessage);

        disconnect(ec, &Executor::processStarted, this, &TaskScheduler::receiveStarted);
        disconnect(ec, &Executor::processPaused, this, &TaskScheduler::receivePaused);
        disconnect(ec, &Executor::processStopped, this, &TaskScheduler::receiveStopped);
        disconnect(ec, &Executor::processEnded, this, &TaskScheduler::receiveEnd);

//        disconnect(es, &EntryScheduler::timerStarted, this, &TaskScheduler::receiveTimerStarted);
//        disconnect(es, &EntryScheduler::timerFinished, this, &TaskScheduler::receiveTimerFinished);
//        disconnect(es, &EntryScheduler::encounteredScheduledTime, this, &TaskScheduler::receiveEncounter);

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

//void TaskScheduler::schedulerStart(QString objectname)
//{
    //TODO: same key value pair needed
//    if(task->contains(objectname) && scheduler->contains(objectname)){
//        EntryTask *et = task->value(objectname);
//        if(et->getPause()){
//            et->pause(false);
//        }else{
//            scheduler->value(objectname)->start();
//        }
//    }

//}

//void TaskScheduler::schedulerStop(QString objectname)
//{
//    removeTask(objectname);
//}

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

//QString TaskScheduler::generateObjectName()
//{
//    QByteArray array = seed.toLocal8Bit();
//    QByteArray result;

//    int arraysize = array.size() - 1;
//    //seedlength*2-2
//    for(int i = 1; i < arraysize; i++){
//        result.append(array.at(i) | array.at(i-1));
//        result.append(array.at(i) & array.at(i+1));
//        if(result >= SEEDLENGTH) break;
//    }

//    QString newseed(result);
//    seed = newseed.left(SEEDLENGTH);
//    return seed;
//}

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
