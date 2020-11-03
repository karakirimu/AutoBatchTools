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

#ifndef SCHEDULERCACHE_H
#define SCHEDULERCACHE_H

#include <QDateTime>
#include <QString>
#include <QUuid>

class SchedulerCache {
public:
    SchedulerCache(){
        settingName = "";
        profilePath = "";
        isScheduled = false;
        scheduleType = 0;
        oneShotDateTime = QDateTime::currentDateTime();
        periodicSeconds = 0;
        everyWeekTime = "00:00:00";
        everyWeekDate = "10000000";
        unique = QUuid::createUuid().toString();
    }
    ~SchedulerCache(){}

    //! Scheduler setting name
    QString settingName;

    //! Selected profile filepath
    QString profilePath;

    //! Schedule state
    bool isScheduled;

    //! Selected schedule type
    int scheduleType;

    //! Scheduled datetime
    QDateTime oneShotDateTime;

    //! Scheduled periodic seconds
    qint64 periodicSeconds;

    //! Scheduled time
    QString everyWeekTime;

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
