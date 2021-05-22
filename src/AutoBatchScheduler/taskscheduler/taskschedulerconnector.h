/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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

    //for syncronize table
    void updateState(int index,
                     QString message,
                     TaskSchedulerConnector::TABLE func);

private slots:
    void initializeTask();

private:
    SchedulerCacheList *cache;

};

#endif // TASKSCHEDULERCONNECTOR_H
