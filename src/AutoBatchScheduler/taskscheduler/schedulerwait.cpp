/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "schedulerwait.h"

SchedulerWait::SchedulerWait(QObject *parent) : QObject(parent)
{
    launchFirst = true;
    running = false;
    connect(this, &SchedulerWait::loop, this, &SchedulerWait::start);
}

SchedulerWait::~SchedulerWait()
{
}

qint64 SchedulerWait::getLestSeconds(QDateTime scheduledDateTime)
{
    return QDateTime::currentDateTime().secsTo(scheduledDateTime);
}

QDateTime SchedulerWait::getNextDateTime(const qint64 &adddays, const QTime &scheduledTime)
{
    QDate cdate = QDate::currentDate().addDays(adddays);
    QDateTime result;
    result.setDate(cdate);
    result.setTime(scheduledTime);

    qDebug() << "[SchedulerWait::getNextDateTime] next : " << result.toString();
    return result;
}

QDateTime SchedulerWait::initScheduledTime()
{
    QDateTime time;
    if(!cache.isScheduled) return time;

    switch(cache.scheduleType){
    case ScheduleType::OneShot:
        time = cache.oneShotDateTime;
        loopschedule = false;
        break;

    case ScheduleType::WeekLoop:
        time = getNextDateTime(getNextDaysCount(cache.everyWeekDate
                                                , cache.everyWeekTime)
                                                , cache.everyWeekTime);
        loopschedule = true;
        break;

    case ScheduleType::PeriodicSecond:
        time = QDateTime::currentDateTime().addSecs(cache.periodicSeconds);
        loopschedule = true;
        break;
    }

    qDebug() << "[SchedulerWait::initScheduledTime] time : " << time;
    return time;
}

QDateTime SchedulerWait::getNextTime(const QDateTime &previous)
{
    QDateTime nexttime;
    if(!cache.isScheduled) return nexttime;

    switch (cache.scheduleType) {
    case ScheduleType::OneShot:
        break;
    case ScheduleType::WeekLoop:
        nexttime = getNextDateTime(getNextDaysCount(cache.everyWeekDate
                                                , cache.everyWeekTime)
                                                , cache.everyWeekTime);
        break;
    case ScheduleType::PeriodicSecond:
        nexttime = previous.addSecs(cache.periodicSeconds);
        break;
    }

    qDebug() << "[SchedulerCalc::getNextTimeFromXml] nexttime : " << nexttime;
    return nexttime;
}

int SchedulerWait::currentDayOfTheWeek()
{
    return QDate::currentDate().dayOfWeek();
}

void SchedulerWait::start()
{
    qint64 lesttime;
    QDateTime scheduledDateTime;

    if(launchFirst){
        scheduledDateTime = initScheduledTime();
        launchFirst = false;
    }else{
        scheduledDateTime = getNextTime(previousTime);
    }
    previousTime = scheduledDateTime;

    qDebug() << "[SchedulerWait::start] started : " << scheduledDateTime;

    //stop timer when first time expired.
    if(getLestSeconds(scheduledDateTime) < 0){
        mutex->lock();
        qDebug() << "[SchedulerWait::start] : Expired : " << cache.objectName();
        emit timerFinished(EXPIRED);
        mutex->unlock();
        return;
    }

    //for restarting
    running = true;

    emit timerStarted(scheduledDateTime);

    while(running){
        lesttime = getLestSeconds(scheduledDateTime);
        if(lesttime > 3000){
            QThread::msleep(slowRefreshTime);
        }else if(lesttime > 0){
            QThread::msleep(fastRefreshTime);
        }else{
            mutex->lock();
            emit encounterScheduledTime();
            mutex->unlock();
            running = false;
        }
    }

    //loop command
    if(loopschedule){
        emit loop();
    }else{
        emit timerFinished(FINISHED);
        launchFirst = true;
    }
}

void SchedulerWait::stop()
{
    running = false;
}

void SchedulerWait::loopstop()
{
    loopschedule = false;
}

int SchedulerWait::getNextDaysCount(const QString &datecode, const QTime &time)
{
    int result = 0;
    int dayofweek = currentDayOfTheWeek();
    int i = dayofweek;
    int loop = 0;
    bool next = true;
    QString charcode;

    while(next){
        loop = i - ((i-1)/7)*7;
        charcode = datecode.at(loop);
        if(charcode.toInt() == 1){
            if(i == dayofweek){
                //current day scheduled
                //this daycount is 7 or 0
                QDateTime rtime;
                rtime.setDate(QDate::currentDate());
                rtime.setTime(time);

                //if not expired today
                if(getLestSeconds(rtime) > 0) next = false;
            }else{
                //other day scheduled
                result = i - dayofweek;
                next = false;
            }
        }
        i++;
    }

    //add weeks
    result += ((static_cast<QString>(datecode.at(0))).toInt() - 1) * 7;

    return result;
}
