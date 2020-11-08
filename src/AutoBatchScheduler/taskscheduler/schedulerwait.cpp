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

#include "schedulerwait.h"

SchedulerWait::SchedulerWait(QObject *parent) : QObject(parent)
{
//    builder = new StartupXmlBuilder();
    launchFirst = true;
    running = false;
    connect(this, &SchedulerWait::loop, this, &SchedulerWait::start);
}

SchedulerWait::~SchedulerWait()
{
//    delete builder;
}

//int SchedulerWait::getSelectedxmlindex() const
//{
//    return selectedxmlindex;
//}

//void SchedulerWait::setSelectedxmlindex(QString objectname)
//{
//    selectedxmlindex = getStartupXmlIndex(objectname);
//}

//QDateTime SchedulerWait::getSchedate() const
//{
//    return schedate;
//}

//bool SchedulerWait::getRunning() const
//{
//    return running;
//}

//qint64 SchedulerWait::getRefreshms() const
//{
//    return refreshms;
//}

//void SchedulerWait::setRefreshms(const qint64 &value)
//{
//    refreshms = value;
//}

//void SchedulerWait::setMutex(QMutex *value)
//{
//    mutex = value;
//}

//qint64 SchedulerWait::getLestSeconds(QString scheduledDateTime)
//{
//    QDateTime scheduled \
//            = QDateTime::fromString(scheduledDateTime, "yyyy/MM/dd HH:mm:ss");
//    return QDateTime::currentDateTime().secsTo(scheduled);
//}

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

//QDateTime SchedulerWait::getNextDateTime(qint64 addseconds)
//{
//    QDateTime result = QDateTime::currentDateTime().addSecs(addseconds);
//    qDebug() << "SchedulerCalc::getNextDateTime(qint64 adddays, QString scheduledTime) :" << result;
//    return result;
//}

QDateTime SchedulerWait::initScheduledTime()
{
//    QList<QStringList> *list = new QList<QStringList>();
    QDateTime time;

//    if(builder->readItem(itemid, list)){
//        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"){
//            switch(((QString)list->at(StartupXmlBuilder::SELECTEDTYPE).at(1)).toInt()){
//            case StartupXmlBuilder::ONESHOT:
//                time = QDateTime::fromString(list->at(StartupXmlBuilder::SC_DATETIME).at(1)
//                                            , "yyyy/MM/dd HH:mm:ss");
//                loopschedule = false;
//                break;

//            case StartupXmlBuilder::WEEKLOOP:
//                time = getNextDateTime(getNextDaysCount(list->at(StartupXmlBuilder::SC_DAY).at(1)
//                                                        , list->at(StartupXmlBuilder::SC_TIME).at(1))
//                                                        , list->at(StartupXmlBuilder::SC_TIME).at(1));
//                loopschedule = true;
//                break;

//            case StartupXmlBuilder::TIMELOOP:
//                time = QDateTime::currentDateTime() \
//                        .addSecs((static_cast<QString>(list->at(5).at(1))).toInt());
//                loopschedule = true;
//                break;

//            default:
//                break;
//            }
//        }
//    }

//    if(!builder->readItem(itemid, list)) return time;

//    if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"){
//        switch((static_cast<QString>(list->at(StartupXmlBuilder::SELECTEDTYPE).at(1))).toInt()){
//        case StartupXmlBuilder::ONESHOT:
//            time = QDateTime::fromString(list->at(StartupXmlBuilder::SC_DATETIME).at(1)
//                                             , "yyyy/MM/dd HH:mm:ss");
//            loopschedule = false;
//            break;

//        case StartupXmlBuilder::WEEKLOOP:
//            time = getNextDateTime(getNextDaysCount(list->at(StartupXmlBuilder::SC_DAY).at(1)
//                                                        , list->at(StartupXmlBuilder::SC_TIME).at(1))
//                                       , list->at(StartupXmlBuilder::SC_TIME).at(1));
//            loopschedule = true;
//            break;

//        case StartupXmlBuilder::TIMELOOP:
//            time = QDateTime::currentDateTime() \
//                        .addSecs((static_cast<QString>(list->at(5).at(1))).toInt());
//            loopschedule = true;
//            break;

//        default:
//            break;
//        }
//    }

//    qDebug() << "select num" << list->at(StartupXmlBuilder::SELECTEDTYPE).at(1);
//    qDebug() << "SchedulerCalc::init_GetScheduledTimeFromXml(int itemid) : " << time;
//    return time;


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
//    QList<QStringList> *list = new QList<QStringList>();
//    QDateTime time;

//    if(builder->readItem(itemid, list)){
//        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"
//            && (static_cast<QString>(list->at(3).at(1))).toInt() == 2){
//            time = current.addSecs((static_cast<QString>(list->at(5).at(1))).toInt());
//        }
//        if(list->at(StartupXmlBuilder::VALID).at(1) == "yes"
//            && (static_cast<QString>(list->at(3).at(1))).toInt() == 1){
//            time = getNextDateTime(getNextDaysCount(list->at(7).at(1)
//                                                    ,list->at(6).at(1))
//                                                    , list->at(6).at(1));
//        }
//    }

//    delete list;
//    qDebug() << "SchedulerCalc::getNextTimeFromXml(int itemid) : " << time;
//    return time;

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

//int SchedulerWait::getStartupXmlIndex(QString objectname)
//{
//    //Copy function. from SystemTray
//    QList<QStringList> *list = new QList<QStringList>();

//    //search valid data
//    int itemid = -1;
//    int count = builder->count();

//    for(int i = 0; i < count; i++){
//        list->clear();
//        if(builder->readItem(i, list)
//                && objectname == list->at(StartupXmlBuilder::UNIQUE).at(1)){
//            itemid = i;
//            break;
//        }
//    }

//    delete list;
//    return itemid;
//}

void SchedulerWait::start()
{
    qint64 lesttime;
    //check null
//    if(selectedxmlindex == -1) return;
    QDateTime scheduledDateTime;

    if(launchFirst){
        scheduledDateTime = initScheduledTime();
        launchFirst = false;
    }else{
        scheduledDateTime = getNextTime(previousTime);
    }
    previousTime = scheduledDateTime;

    //instruct schedule
//    if(schedate.isValid()){
//        scheduledDateTime = schedate;
//    }else{
//    mutex->lock();
//    scheduledDateTime = initScheduledTime();
//    schedate = scheduledDateTime;
//    mutex->unlock();
//    }

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

//int SchedulerCalc::dateCheckedCounter(QString datecode)
//{
//    int count = 0;
//    for(int i = 1; i < WEEKDATA; i++){
//        if((QString)(datecode.at(i)).toInt() == 1) count++;
//    }
//    return count;
//}
