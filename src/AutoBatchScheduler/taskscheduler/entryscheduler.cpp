/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "entryscheduler.h"

EntryScheduler::EntryScheduler(QObject *parent) : QObject(parent)
{
    swait = new SchedulerWait();

    //set internal thread
    worker = new QThread();

    //connect thread
    connect(worker, &QThread::started, swait, &SchedulerWait::start);

    swait->moveToThread(worker);
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
}
