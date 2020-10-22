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

#include "multitaskp.h"

MultiTaskP::MultiTaskP(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, RunTaskSignalBinder *>();
    basemutex = new QMutex();

    //time based seed set
    QTime *ti = new QTime();
//    qsrand(static_cast<uint>(ti->currentTime().msecsSinceStartOfDay()) ^ 123456789);
    delete ti;
}

MultiTaskP::~MultiTaskP()
{
    //delete all left tasks
    QHash<QString, RunTaskSignalBinder *>::iterator i = task->begin();
    while (i != task->end()) {
        removeTask(i.key());
        ++i;
    }

    delete task;
    delete basemutex;
}

bool MultiTaskP::taskRunningCheck(QString objectname)
{
    bool result = false;
    if(processAliveCheck(objectname)){
        RunTaskSignalBinder *et = task->value(objectname);
        result = et->getStarted();
    }

    return result;
}

void MultiTaskP::addTask(QString objectname, QString processfile)
{
    //new task set
    RunTaskSignalBinder *et = new RunTaskSignalBinder();

    //connect message
    connect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTaskP::receiveInitCount);
    connect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTaskP::receiveCurrent);
    connect(et, &RunTaskSignalBinder::processError, this, &MultiTaskP::receiveError);
    connect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTaskP::receiveErrorText);
    connect(et, &RunTaskSignalBinder::processMessage, this, &MultiTaskP::receiveMessage);
    connect(et, &RunTaskSignalBinder::processStarted, this, &MultiTaskP::receiveStarted);
    connect(et, &RunTaskSignalBinder::processPaused, this, &MultiTaskP::receivePaused);
    connect(et, &RunTaskSignalBinder::processStopped, this, &MultiTaskP::receiveStopped);
    connect(et, &RunTaskSignalBinder::processEnd, this, &MultiTaskP::receiveEnd);

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

void MultiTaskP::removeTask(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    //stop process
    et->stop();

    //disconnect child object
    disconnect(et, &RunTaskSignalBinder::processInitCount, this, &MultiTaskP::receiveInitCount);
    disconnect(et, &RunTaskSignalBinder::processCurrent, this, &MultiTaskP::receiveCurrent);
    disconnect(et, &RunTaskSignalBinder::processError, this, &MultiTaskP::receiveError);
    disconnect(et, &RunTaskSignalBinder::processErrorText, this, &MultiTaskP::receiveErrorText);
    disconnect(et, &RunTaskSignalBinder::processMessage, this, &MultiTaskP::receiveMessage);
    disconnect(et, &RunTaskSignalBinder::processStarted, this, &MultiTaskP::receiveStarted);
    disconnect(et, &RunTaskSignalBinder::processPaused, this, &MultiTaskP::receivePaused);
    disconnect(et, &RunTaskSignalBinder::processStopped, this, &MultiTaskP::receiveStopped);
    disconnect(et, &RunTaskSignalBinder::processEnd, this, &MultiTaskP::receiveEnd);

    //delete register
    task->remove(objectname);

    delete et;
}

void MultiTaskP::setInputFileList(QString objectname, QStringList *list)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    et->updateFileList(list);
}

void MultiTaskP::setRange(QString objectname, QString str)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(str != "") et->updateRange(str);
}

void MultiTaskP::processPause(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    if(et->getStarted() && !et->getPause()){
        et->pause(true);
    }else{
        et->pause(false);
    }
}

void MultiTaskP::processStop(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->stop();
}

void MultiTaskP::sendInput(QString objectname, QString text)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);
    if(et->getStarted()) et->sendInput(text);
}

bool MultiTaskP::processAliveCheck(QString objectname)
{
    return task->contains(objectname) ? true : false;
}

QString MultiTaskP::generateRandom(int length)
{
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
