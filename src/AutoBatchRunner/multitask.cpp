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

#include "multitask.h"

MultiTask::MultiTask(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, EntryTask *>();
    basemutex = new QMutex();

    //time based seed set
//    QTime *ti = new QTime();
//    rgen = QRandomGenerator(static_cast<uint>(ti->currentTime().msecsSinceStartOfDay() ^ 123456789));
//    qsrand(static_cast<uint>(ti->currentTime().msecsSinceStartOfDay() ^ 123456789));
//    delete ti;
}

MultiTask::~MultiTask()
{
    //delete all left tasks
    QHash<QString, EntryTask *>::iterator i = task->begin();
    while (i != task->end()) {
        removeTask(i.key());
        ++i;
    }

    delete task;
    delete basemutex;
}

bool MultiTask::taskRunningCheck(QString objectname)
{
    bool result = false;
    if(processAliveCheck(objectname)){
        EntryTask *et = task->value(objectname);
        result = et->getStarted();
    }

    return result;
}

void MultiTask::addTask(QString objectname, QString processfile)
{
    //new task set
    EntryTask *et = new EntryTask();

    //connect message
    connect(et, &EntryTask::processInitCount, this, &MultiTask::receiveInitCount);
    connect(et, &EntryTask::processCurrent, this, &MultiTask::receiveCurrent);
    connect(et, &EntryTask::processError, this, &MultiTask::receiveError);
    connect(et, &EntryTask::processErrorText, this, &MultiTask::receiveErrorText);
    connect(et, &EntryTask::processMessage, this, &MultiTask::receiveMessage);
    connect(et, &EntryTask::processStarted, this, &MultiTask::receiveStarted);
    connect(et, &EntryTask::processPaused, this, &MultiTask::receivePaused);
    connect(et, &EntryTask::processStopped, this, &MultiTask::receiveStopped);
    connect(et, &EntryTask::processEnd, this, &MultiTask::receiveEnd);

    //set task filename
    et->setFile(processfile);

    //set same objectname
    et->setObjectName(objectname);

    //set shared mutex (for secure running)
    et->setMutex(basemutex);

    //add pointer
    task->insert(objectname, et);

    //start task
    et->start();
}

void MultiTask::removeTask(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    EntryTask *et = task->value(objectname);

    //stop process
    et->stop();

    //disconnect child object
    disconnect(et, &EntryTask::processInitCount, this, &MultiTask::receiveInitCount);
    disconnect(et, &EntryTask::processCurrent, this, &MultiTask::receiveCurrent);
    disconnect(et, &EntryTask::processError, this, &MultiTask::receiveError);
    disconnect(et, &EntryTask::processErrorText, this, &MultiTask::receiveErrorText);
    disconnect(et, &EntryTask::processMessage, this, &MultiTask::receiveMessage);
    disconnect(et, &EntryTask::processStarted, this, &MultiTask::receiveStarted);
    disconnect(et, &EntryTask::processPaused, this, &MultiTask::receivePaused);
    disconnect(et, &EntryTask::processStopped, this, &MultiTask::receiveStopped);
    disconnect(et, &EntryTask::processEnd, this, &MultiTask::receiveEnd);

    //delete register
    task->remove(objectname);

    delete et;
}

void MultiTask::setInputFileList(QString objectname, QStringList *list)
{
    if(!processAliveCheck(objectname)) return;
    EntryTask *et = task->value(objectname);
    et->updateFileList(list);
}

void MultiTask::processPause(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    EntryTask *et = task->value(objectname);

    if(et->getStarted() && !et->getPause()){
        et->pause(true);
    }else{
        et->pause(false);
    }
}

void MultiTask::processStop(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    EntryTask *et = task->value(objectname);
    if(et->getStarted()) et->stop();
}

void MultiTask::sendInput(QString objectname, QString text)
{
    if(!processAliveCheck(objectname)) return;
    EntryTask *et = task->value(objectname);
    if(et->getStarted()) et->sendInput(text);
}

bool MultiTask::processAliveCheck(QString objectname)
{
    return task->contains(objectname) ? true : false;
}

QString MultiTask::generateRandom(int length)
{
    //Fisher-Yates
    QString characters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.");

    //shuffle characters
    int pos = characters.size() - 1;
    int random;
    while(pos > 1){
        random = abs(static_cast<int>(QRandomGenerator::global()->generate()) % pos);
        QChar tmp = characters.at(random);
        characters.replace(random, 1, characters.at(pos));
        characters.replace(pos, 1, tmp);
        pos--;
    }

    //select characters
    QString randomString;
    for(int i=0; i < length; ++i)
    {
        int index = abs(static_cast<int>(QRandomGenerator::global()->generate()) % length);
        QChar nextChar = characters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
