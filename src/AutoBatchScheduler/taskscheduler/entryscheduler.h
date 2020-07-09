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

#ifndef TIMESCHEDULERTHREAD_H
#define TIMESCHEDULERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <schedulerwait.h>
#include <startupxmlbuilder.h>

class EntryScheduler : public QObject
{
    Q_OBJECT
public:
    explicit EntryScheduler(QObject *parent = nullptr);
    ~EntryScheduler();

    bool isStarted();
    void setRefreshTime(int sec); //preset
//    void setXmlItemId(int itemid); //preset
    void setMutex(QMutex *mutex); //preset

signals:
    void timerStarted(QDateTime);
    void timerFinished(int);
    void encounteredScheduledTime();

public slots:
    void start();
    void stop();

    //receive
    void receiveStarted(QDateTime datetime){emit timerStarted(datetime);}
    void receiveFinished(int value){emit timerFinished(value);}
    void receiveEncounter(){emit encounteredScheduledTime();}

    void setXmlItemId(QString objname);

private:
    SchedulerWait *swait;
    QThread *worker;
};

#endif // TIMESCHEDULERTHREAD_H
