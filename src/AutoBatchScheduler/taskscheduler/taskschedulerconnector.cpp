/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "taskschedulerconnector.h"

TaskSchedulerConnector::TaskSchedulerConnector(QObject *parent)
    : TaskScheduler(parent)
{
    cache = new SchedulerCacheList();
    connect(this, &TaskSchedulerConnector::fileLoadCompleted, \
            this, &TaskSchedulerConnector::initializeTask);
}

TaskSchedulerConnector::~TaskSchedulerConnector()
{
    saveFile();

    QList<SchedulerCache> list = readAll();

    // disable all left tasks
    int i = 0;
    for(const SchedulerCache& sc : list){
        if(sc.isScheduled){
            removeTask(sc.objectName());
        }
        i++;
    }

    delete cache;
}

/**
 * @brief Read cache data corresponding to index
 * @param index Table row position that you want to get.
 * @return Object of specified index or blank
 */
const SchedulerCache TaskSchedulerConnector::read(int index)
{
    if(index < 0 || cache->count() <= index){
        return SchedulerCache();
    }

    return SchedulerCache(cache->at(index));
}

/**
 * @brief Read all cache data
 * @return cache data
 */
const QList<SchedulerCache> TaskSchedulerConnector::readAll()
{
    return cache->getCacheCopy();
}

bool TaskSchedulerConnector::saveFile()
{
    QList<QList<QStringList> *> list;
    cache->serialize(&list);

    StartupXmlBuilder builder;
    if(builder.writeAll(&list)){
        return true;
    }

    return false;
}

bool TaskSchedulerConnector::loadFile()
{
    StartupXmlBuilder builder;
    QList<QList<QStringList> *> list;
    if(builder.readAll(&list)){
        cache->deSerialize(&list);
        emit fileLoadCompleted();
        return true;
    }

    return false;
}

void TaskSchedulerConnector::add(const SchedulerCache &sc)
{
    int index = cache->count();
    cache->append(sc);
    emit updateState(index, sc.objectName(), ADD);
}

void TaskSchedulerConnector::insert(int index, const SchedulerCache &sc)
{
    cache->insert(index, sc);
    emit updateState(index, sc.objectName(), INSERT);
}

void TaskSchedulerConnector::remove(int index)
{
    SchedulerCache sc = cache->at(index);
    if(sc.isScheduled){
        removeTask(sc.objectName());
    }
    cache->removeAt(index);
    emit updateState(index, sc.objectName(), DELETE);
}

void TaskSchedulerConnector::replace(int index, const SchedulerCache &sc)
{
    cache->replace(index, sc);
    emit updateState(index, sc.objectName(), EDIT);
}

void TaskSchedulerConnector::duplicate(int index)
{
    SchedulerCache sc(cache->at(index), true);
    sc.isScheduled = false;
    cache->append(sc);
    emit updateState(index + 1, "", DUPLICATE);
}

void TaskSchedulerConnector::enableSchedule(int index)
{
    SchedulerCache sc = cache->at(index);
    QFileInfo info(sc.profilePath);
    if(!sc.isScheduled && info.exists()){
        sc.isScheduled = true;
        addTask(sc);
        cache->replace(index, sc);
        emit updateState(index, sc.objectName(), ENABLE);
    }

    if(!info.exists())
        qDebug() << "[TaskSchedulerConnector::enableSchedule] file is not exist.";
}

void TaskSchedulerConnector::disableSchedule(int index)
{
    SchedulerCache sc = cache->at(index);
    if(sc.isScheduled){
        sc.isScheduled = false;
        removeTask(sc.objectName());
        cache->replace(index, sc);
        emit updateState(index, sc.objectName(), DISABLE);
    }
}

void TaskSchedulerConnector::move(int from, int to)
{
    cache->move(from, to);
    emit updateState(from, QString::number(to), MOVE);
}

void TaskSchedulerConnector::initializeTask()
{
    QList<SchedulerCache> list = readAll();

    int i = 0;
    for(const SchedulerCache& sc : list){
        QFileInfo info(sc.profilePath);
        if(sc.isScheduled && info.exists()){
            addTask(sc);
            emit updateState(i, sc.objectName(), ENABLE);
        }
        i++;
    }
}
