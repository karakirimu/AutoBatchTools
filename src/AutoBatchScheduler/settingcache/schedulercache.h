/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SCHEDULERCACHE_H
#define SCHEDULERCACHE_H

#include <QDateTime>
#include <QString>
#include <QUuid>

enum class ScheduleType {
    OneShot,
    WeekLoop,
    PeriodicSecond
};

class SchedulerCache {
public:
    explicit SchedulerCache(){
        settingName = "";
        profilePath = "";
        isScheduled = false;
        scheduleType = ScheduleType::OneShot;
        oneShotDateTime = QDateTime::currentDateTime();
        periodicSeconds = 0;
        everyWeekTime = QTime::currentTime();
        everyWeekDate = "10000000";
        unique = QUuid::createUuid().toString();
    }

    SchedulerCache (const SchedulerCache &other, bool uuidupdate){
        this->settingName = other.settingName;
        this->profilePath = other.profilePath;
        this->isScheduled = other.isScheduled;
        this->scheduleType = other.scheduleType;
        this->oneShotDateTime = other.oneShotDateTime;
        this->periodicSeconds = other.periodicSeconds;
        this->everyWeekTime = other.everyWeekTime;
        this->everyWeekDate = other.everyWeekDate;
        this->unique = uuidupdate ? QUuid::createUuid().toString() : other.unique;
    }

    ~SchedulerCache(){}

    //! Scheduler setting name
    QString settingName;

    //! Selected profile filepath
    QString profilePath;

    //! Schedule state
    bool isScheduled;

    //! Selected schedule type
    ScheduleType scheduleType;

    //! Scheduled datetime
    QDateTime oneShotDateTime;

    //! Scheduled periodic seconds
    qint64 periodicSeconds;

    //! Scheduled time
    QTime everyWeekTime;

    //! Scheduled day of the week and weeks
    //!
    //! data structure
    //! weeks: 1 or bigger than 1 (0 is invalid)
    //! day : enabled = 1, disabled = 0
    //! weeks every :monday :tuesday :wednesday :thursday :friday :saturday :sunday :
    //!
    //! example:
    //! 20011010
    //! 2:0:0:1:1:0:1:0:
    //! every weeks = 2, mon = 0, tue = 0, wed = 1, thu = 1, fri = 0, sat = 1, sun = 0
    QString everyWeekDate;


    inline QString objectName() const { return unique; }

private:
    //! Auto generated unique parameter
    QString unique;

};

#endif // SCHEDULERCACHE_H
