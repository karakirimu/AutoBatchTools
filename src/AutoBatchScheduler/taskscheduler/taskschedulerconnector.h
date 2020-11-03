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

class TaskSchedulerConnector : public TaskScheduler
{
    Q_OBJECT
public:
    explicit TaskSchedulerConnector(QObject *parent = nullptr);
    ~TaskSchedulerConnector();

    //! table function
    enum class TABLE {ADD,EDIT,INSERT,DELETE,ENABLE,DISABLE,COPY,DRAGDROP,UP,DOWN,SWAP};

signals:
    void taskDisabled(QString);
    void taskEnabled(QString);

    //for syncronize table
    //startuptable <-> processshowtable
    void tableMessenger(QString message, TABLE func);

public slots:
    //from systemtray or settingdialog
    void enableTask(QString objectname, QString filepath); //set objectname
    void disableTask(QString objectname);

    //from startuptable
//    void actionAdded(int itemid){emit addListAction(itemid);}
//    void actionDeleted(QString objectname){emit deleteListAction(objectname);}

};

#endif // TASKSCHEDULERCONNECTOR_H
