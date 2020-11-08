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

#ifndef SCHEDULERWAIT_H
#define SCHEDULERWAIT_H

#include <../variantconverter/variantconverter.h>
#include <QObject>
#include <QMutex>
#include <QThread>
#include <QDateTime>
#include <settingcache/schedulercache.h>
#include <QDebug>
//#include <settingcache/startupxmlbuilder.h>

//#define WEEKDATA 8

class SchedulerWait : public QObject
{
    Q_OBJECT
public:
    explicit SchedulerWait(QObject *parent = nullptr);
    ~SchedulerWait();

    inline void setMutex(QMutex *value){ mutex = value; }

//    inline ulong getRefreshms() const { return slowRefreshTime; }
    inline void setRefreshms(const ulong &value){ slowRefreshTime = value; }

    inline bool getRunning() const { return running; }

//    inline QDateTime getSchedate() const { return schedate; }

//    inline int getSelectedxmlindex() const { return selectedxmlindex; }
//    inline void setSelectedxmlindex(QString objectname){
//        selectedxmlindex = getIndex(objectname);
//    }

    inline void setSchedulerCache(const SchedulerCache &sc ){ cache = SchedulerCache(sc, false); }

    enum{FINISHED,EXPIRED};
signals:
    //send to ui
    void timerStarted(QDateTime);

    //below functions emitted, deleted this task
    void timerFinished(int);

    void encounterScheduledTime();

    //internal signal
    void loop();
public slots:
    //for timer
    void start();
    void stop();
    void loopstop();

private:
    //inner calc functions
//    [[deprecated]]qint64 getLestSeconds(QString scheduledDateTime);
    qint64 getLestSeconds(QDateTime scheduledDateTime);
//    [[deprecated]]QDateTime getNextDateTime(qint64 addseconds);
    QDateTime initScheduledTime();
    QDateTime getNextTime(const QDateTime &previous);
    int currentDayOfTheWeek();

    int getNextDaysCount(const QString &datecode, const QTime &time);
//    int dateCheckedCounter(QString datecode);
    QDateTime getNextDateTime(const qint64 &adddays, const QTime &scheduledTime);

//    int getStartupXmlIndex(QString objectname);

//    StartupXmlBuilder *builder;
    SchedulerCache cache;

    //variant for manage
    ulong slowRefreshTime = 1000;
    const ulong fastRefreshTime = 100;
//    int selectedxmlindex = -1;
//    QDateTime schedate;
    QDateTime previousTime;
    QMutex *mutex;

    std::atomic<bool> launchFirst;
    std::atomic<bool> running;

    //when schedule loop
    bool loopschedule = false;
};

#endif // SCHEDULERWAIT_H
