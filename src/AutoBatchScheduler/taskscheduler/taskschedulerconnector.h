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

#ifndef TASKSCHEDULERCONNECTOR_H
#define TASKSCHEDULERCONNECTOR_H

#include "taskscheduler.h"

#include <settingcache/schedulercachelist.h>

class TaskSchedulerConnector : public TaskScheduler
{
    Q_OBJECT
public:
    explicit TaskSchedulerConnector(QObject *parent = nullptr);
    ~TaskSchedulerConnector();

    const SchedulerCache read(int index);
    const QList<SchedulerCache> readAll();

    // xml output
    bool saveFile();
    bool loadFile();

    //! table function
    enum TABLE {ADD,EDIT,INSERT,DELETE,ENABLE,DISABLE,DUPLICATE,DRAGDROP,UP,DOWN,SWAP,MOVE};

//    void updateCache(int index, TABLE func);

    void add(const SchedulerCache &sc);
    void insert(int index, const SchedulerCache &sc);
    void remove(int index);
    void replace(int index, const SchedulerCache &sc);
    void duplicate(int index);

    void enableSchedule(int index);
    void disableSchedule(int index);

    void move(int from, int to);

signals:
    void fileLoadCompleted();
//    void taskDisabled(QString);
//    void taskEnabled(QString);

    //for syncronize table
    //startuptable <-> processshowtable
//    void tableMessenger(int index, TABLE func);
    void updateState(int index, QString message, TABLE func);

//public slots:
    //from systemtray or settingdialog
//    void enableTask(QString objectname, QString filepath); //set objectname
//    void disableTask(QString objectname);

private slots:
    void initializeTask();

private:
    SchedulerCacheList *cache;

};

#endif // TASKSCHEDULERCONNECTOR_H
