/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "multitask.h"

MultiTask::MultiTask(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, EntryTask *>();
    basemutex = new QMutex();
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

    //connect child object
    Executor *ec = et->getExecutor();
    connect(ec, &Executor::processStateCount, this, &MultiTask::receiveInitCount);
    connect(ec, &Executor::processStateUpdate, this, &MultiTask::receiveCurrent);
    connect(ec, &Executor::processCheckError, this, &MultiTask::receiveErrorText);
    connect(ec, &Executor::processMessage, this, &MultiTask::receiveMessage);

    connect(ec, &Executor::processStarted, this, &MultiTask::receiveStarted);
    connect(ec, &Executor::processPaused, this, &MultiTask::receivePaused);
    connect(ec, &Executor::processStopped, this, &MultiTask::receiveStopped);
    connect(ec, &Executor::processEnded, this, &MultiTask::receiveEnd);

    //set task filename
    et->setFile(processfile);

    //set same objectname
    et->setObjectName(objectname);
    ec->setObjectName(objectname);

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
    const Executor *ec = et->getExecutor();
    disconnect(ec, &Executor::processStateCount, this, &MultiTask::receiveInitCount);
    disconnect(ec, &Executor::processStateUpdate, this, &MultiTask::receiveCurrent);
    disconnect(ec, &Executor::processCheckError, this, &MultiTask::receiveErrorText);
    disconnect(ec, &Executor::processMessage, this, &MultiTask::receiveMessage);

    disconnect(ec, &Executor::processStarted, this, &MultiTask::receiveStarted);
    disconnect(ec, &Executor::processPaused, this, &MultiTask::receivePaused);
    disconnect(ec, &Executor::processStopped, this, &MultiTask::receiveStopped);
    disconnect(ec, &Executor::processEnded, this, &MultiTask::receiveEnd);

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
