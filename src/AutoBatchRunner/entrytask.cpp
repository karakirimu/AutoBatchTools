/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "entrytask.h"

EntryTask::EntryTask(QObject *parent)
    : QObject(parent)
{
    //setmain worker
    executor = new Executor();

    //set internal thread
    worker = new QThread();

    //init executor const
    executor->setLaunchedfrom(Executor::DEFAULT);

    //connect thread to doWork
    connect(worker, &QThread::started, executor, &Executor::runProcess);
    connect(worker, &QThread::finished, this, &EntryTask::processCompleted);
    connect(executor, &Executor::processEnded, worker, &QThread::quit);
    connect(executor, &Executor::processStopped, worker, &QThread::quit);

    //move to thread
    executor->moveToThread(worker);
}

EntryTask::~EntryTask()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete executor;
}

void EntryTask::setFile(QString filepath)
{
    qDebug() << "[EntryTask::setFile] File Changed :" << filepath;
    executor->setProcessFile(filepath);
}

void EntryTask::setMutex(QMutex *sharedmutex)
{
    executor->setMutex(sharedmutex);
}

bool EntryTask::getStarted()
{
    return executor->getWorking();
}

bool EntryTask::getPause()
{
    return executor->getPaused();
}

void EntryTask::sendInput(QString message)
{
    if(executor->getWorking())
        executor->processWrite(message);
}

void EntryTask::start()
{
    if(executor->getPaused()){
        executor->setPaused(false);

    }else{
        //load static settings
        executor->setGlobalList();
        executor->setLocalList();

        worker->start();
    }
}

void EntryTask::pause(bool paused)
{
    executor->setPaused(paused);
}

void EntryTask::stop()
{
    executor->stopProcess();
}

void EntryTask::updateFileList(QStringList *need)
{
    if(need->empty()) return;
    executor->addInputFiles(*need, -1);
}

void EntryTask::processCompleted()
{
    qDebug() << "[EntryTask::processCompleted]";
}
