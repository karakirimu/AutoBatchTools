/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "multitaskp.h"

MultiTaskP::MultiTaskP(QObject *parent)
    : QObject(parent)
{
    task = new QHash<QString, RunTaskSignalBinder *>();
    basemutex = new QMutex();
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
    Executor *ec = et->getExecutor();
    connect(ec, &Executor::processStateCount, this, &MultiTaskP::receiveInitCount);
    connect(ec, &Executor::processStateUpdate, this, &MultiTaskP::receiveCurrent);
    connect(ec, &Executor::processCheckError, this, &MultiTaskP::receiveErrorText);
    connect(ec, &Executor::processMessage, this, &MultiTaskP::receiveMessage);

    connect(ec, &Executor::processStarted, this, &MultiTaskP::receiveStarted);
    connect(ec, &Executor::processPaused, this, &MultiTaskP::receivePaused);
    connect(ec, &Executor::processStopped, this, &MultiTaskP::receiveStopped);
    connect(ec, &Executor::processEnded, this, &MultiTaskP::receiveEnd);

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

void MultiTaskP::removeTask(QString objectname)
{
    if(!processAliveCheck(objectname)) return;
    RunTaskSignalBinder *et = task->value(objectname);

    //stop process
    et->stop();

    //disconnect child object
    const Executor *ec = et->getExecutor();
    disconnect(ec, &Executor::processStateCount, this, &MultiTaskP::receiveInitCount);
    disconnect(ec, &Executor::processStateUpdate, this, &MultiTaskP::receiveCurrent);
    disconnect(ec, &Executor::processCheckError, this, &MultiTaskP::receiveErrorText);
    disconnect(ec, &Executor::processMessage, this, &MultiTaskP::receiveMessage);

    disconnect(ec, &Executor::processStarted, this, &MultiTaskP::receiveStarted);
    disconnect(ec, &Executor::processPaused, this, &MultiTaskP::receivePaused);
    disconnect(ec, &Executor::processStopped, this, &MultiTaskP::receiveStopped);
    disconnect(ec, &Executor::processEnded, this, &MultiTaskP::receiveEnd);

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
