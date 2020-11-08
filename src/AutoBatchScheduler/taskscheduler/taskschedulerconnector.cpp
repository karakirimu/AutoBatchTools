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
    for(SchedulerCache sc : list){
        if(sc.isScheduled){
            removeTask(sc.objectName());
//            emit updateState(i, sc.objectName(), DISABLE);
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
    for(SchedulerCache sc : list){
        QFileInfo info(sc.profilePath);
        if(sc.isScheduled && info.exists()){
            addTask(sc);
            emit updateState(i, sc.objectName(), ENABLE);
        }
        i++;
    }
}

//void TaskSchedulerConnector::enableTask(QString objectname, QString filepath)
//{
//    scheduler->addTask(objectname, filepath);
//    emit taskEnabled(objectname);
//}

//void TaskSchedulerConnector::disableTask(QString objectname)
//{
//    scheduler->removeTask(objectname);
//    emit taskDisabled(objectname);
//}

//void TaskSchedulerConnector::updateCache(int index
//                                         , TaskSchedulerConnector::TABLE func)
//{
//    switch (func) {
//    case TABLE::DELETE:
//        cache->removeAt(index);
//        break;
//    case TABLE::UP:
//        cache->move(index, index-1);
//        break;
//    case TABLE::DOWN:
//        cache->move(index, index+1);
//        break;
//    case TABLE::COPY:
//    {
//        SchedulerCache sc(cache->at(index), true);
//        cache->insert(index,sc);
//        break;
//    }
//    case TABLE::ENABLE:
//    {
//        SchedulerCache sc = cache->at(index);
//        if(!sc.isScheduled){
//            sc.isScheduled = true;
//            cache->replace(index, sc);
//            emit updateSchedulerState(index, cache->at(index).objectName(), ENABLE);
//        }
//        break;
//    }
//    case TABLE::DISABLE:
//    {
//        SchedulerCache sc = cache->at(index);
//        if(sc.isScheduled){
//            sc.isScheduled = false;
//            cache->replace(index, sc);
//            emit updateSchedulerState(index, cache->at(index).objectName(), DISABLE);
//        }
//        break;
//    }
//    case TABLE::ADD:
//    case TABLE::INSERT:
//    case TABLE::EDIT:
//    case TABLE::SWAP:
//    case TABLE::DRAGDROP:
//        break;
//    }
//}
