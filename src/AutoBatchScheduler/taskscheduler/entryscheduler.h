/*
 * Copyright 2016-2021 karakirimu
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

#ifndef ENTRYSCHEDULER_H
#define ENTRYSCHEDULER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <schedulerwait.h>
#include <settingcache/startupxmlbuilder.h>

class EntryScheduler : public QObject
{
    Q_OBJECT
public:
    explicit EntryScheduler(QObject *parent = nullptr);
    ~EntryScheduler();

    bool isStarted();
    void setRefreshTime(ulong milliseconds); //preset
    void setMutex(QMutex *mutex); //preset

    SchedulerWait *getSchedulerWait() const { return swait; }

public slots:
    void start();
    void stop();

private:
    SchedulerWait *swait;
    QThread *worker;

};

#endif // ENTRYSCHEDULER_H
