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

#include "runtasksignalbinder.h"

#include <QSettings>

RunTaskSignalBinder::RunTaskSignalBinder(QObject *parent)
    : QObject(parent)
{
    //set internal thread
    worker = new QThread();

    //setmain worker
    executor = new Executor();

    //connect thread to doWork
    connect(worker, &QThread::started, executor, &Executor::runProcess);
    connect(executor, &Executor::processEnded, worker, &QThread::quit);
    connect(executor, &Executor::processStopped, worker, &QThread::quit);

    //connect executor
    connect(executor, &Executor::processStarted, this, &RunTaskSignalBinder::receiveStarted);
    connect(executor, &Executor::processEnded, this, &RunTaskSignalBinder::receiveEnd);
    connect(executor, &Executor::processStopped, this, &RunTaskSignalBinder::receiveStopped);
    connect(executor, &Executor::processPaused, this, &RunTaskSignalBinder::receivePaused);

    connect(executor, &Executor::processStateUpdate, this, &RunTaskSignalBinder::receiveCurrent);
    connect(executor, &Executor::processStateCount, this, &RunTaskSignalBinder::receiveInitCount);
    connect(executor, &Executor::processCheckError, this, &RunTaskSignalBinder::receiveErrorText);
    connect(executor, &Executor::processMessage, this, &RunTaskSignalBinder::receiveMessage);

    //move to thread
    executor->moveToThread(worker);
}

RunTaskSignalBinder::~RunTaskSignalBinder()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait();
    }
    delete worker;
    delete executor;
}

bool RunTaskSignalBinder::getStarted()
{
    return executor->getWorking();
}

bool RunTaskSignalBinder::getPause()
{
    return executor->getPaused();
}

void RunTaskSignalBinder::setMutex(QMutex *sharedmutex)
{
    executor->setMutex(sharedmutex);
}

void RunTaskSignalBinder::setFile(QString filepath)
{
    qDebug() << "RunTaskSignalBinder:: File Changed :" << filepath;
    executor->setProcessFile(filepath);
}

void RunTaskSignalBinder::sendInput(QString message)
{
    if(executor->getWorking()) executor->processWrite(message);
}

void RunTaskSignalBinder::start()
{
    if(executor->getPaused()){
        executor->setPaused(false);

    }else{
        //load static settings
        QSettings settings( "./settings.ini", QSettings::IniFormat );

        settings.beginGroup("pe_testexec");
        executor->setLaunchedfrom(settings.value("FAKERES", false).toInt());
        settings.endGroup();

        worker->start();
    }
}

void RunTaskSignalBinder::pause(bool pause)
{
    executor->setPaused(pause);
}

void RunTaskSignalBinder::stop()
{
    executor->stopProcess();
//    executor->processKill();
}

//tested multiple range (x,x,x,x)
void RunTaskSignalBinder::updateRange(QString str)
{
    QStringList split = str.split(",");
    QStringList tmp;
    int tmpstart = 0;
    int tmpend = 0;
    QList<int> result;
    foreach(QString var, split){
        if(var.contains("-")){
            tmp.clear();
            tmp = var.split("-");

            tmpstart = static_cast<QString>(tmp.at(0)).toInt();

            //error correction
            tmpstart = tmpstart < 2 ? 0 : tmpstart;

            tmpend = static_cast<QString>(tmp.at(1)).toInt();

            for(; tmpstart <= tmpend; tmpstart++){
                result.append(tmpstart);
            }

        }else{
            result.append(var.toInt() > 1 ? var.toInt() : 0);
        }
    }

    executor->setExecList(result);
}

void RunTaskSignalBinder::updateFileList(QStringList *data)
{
    if(data->empty()) return;
    executor->addInputFiles(*data, -1);

}
