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

#include "schedulercacheconverter.h"

SchedulerCacheConverter::SchedulerCacheConverter()
{

}

SchedulerCacheConverter::~SchedulerCacheConverter()
{

}

void SchedulerCacheConverter::convertToSchedulerCache(const QList<QList<QStringList> *> *source
                                                   , QList<SchedulerCache> *dest)
{
    dest->clear();
    for(QList<QStringList> *one : *source){
        SchedulerCache cache;
        convertToCache(&cache,one);
        dest->append(cache);
    }
}

void SchedulerCacheConverter::convertToXml(const QList<SchedulerCache> *source
                                           , QList<QList<QStringList> *> *dest)
{
    dest->clear();
    for(SchedulerCache cache : *source){
        QList<QStringList> *one = new QList<QStringList>();
        convertFromCache(&cache, one);
        dest->append(one);
    }
}

void SchedulerCacheConverter::convertFromCache(const SchedulerCache *from
                                               , QList<QStringList> *to)
{
    to->append(QStringList() << sxc.TAG_NAME << from->settingName);
    to->append(QStringList() << sxc.TAG_PROFILE << from->profilePath);
    to->append(QStringList() << sxc.TAG_SCHEDULED \
                                << VariantConverter::boolToString(from->isScheduled));
    to->append(QStringList() << sxc.TAG_SCHEDULETYPE \
                                << QString::number(static_cast<int>(from->scheduleType)));
    to->append(QStringList() << sxc.TAG_DATETIME \
                                << from->oneShotDateTime.toString(DATE_FORMAT));
    to->append(QStringList() << sxc.TAG_SECOND \
                                << QString::number(from->periodicSeconds));
    to->append(QStringList() << sxc.TAG_TIME << from->everyWeekTime.toString(TIME_FORMAT));
    to->append(QStringList() << sxc.TAG_DATE << from->everyWeekDate);
//    to->append(QStringList() << sxc.TAG_UNIQUE << from->objectName());
}

void SchedulerCacheConverter::convertToCache(SchedulerCache *to
                                             , const QList<QStringList> *from)
{
    to->settingName = fetch(sxc.TAG_NAME, from);
    to->profilePath = fetch(sxc.TAG_PROFILE, from);
    to->isScheduled = VariantConverter::stringToBool(fetch(sxc.TAG_SCHEDULED, from));
    to->scheduleType = static_cast<ScheduleType>(fetch(sxc.TAG_SCHEDULETYPE, from).toInt());
    to->oneShotDateTime = QDateTime::fromString(fetch(sxc.TAG_DATETIME, from), DATE_FORMAT);
    to->periodicSeconds = fetch(sxc.TAG_SECOND, from).toLongLong();
    to->everyWeekTime = QTime::fromString(fetch(sxc.TAG_TIME, from), TIME_FORMAT);
    to->everyWeekDate = fetch(sxc.TAG_DATE, from);
}

QString SchedulerCacheConverter::fetch(QString tag
                                       , const QList<QStringList> *loadbase)
{
    return this->fetch(tag, "", loadbase);
}

QString SchedulerCacheConverter::fetch(QString tag
                                       , QString attr
                                       , const QList<QStringList> *loadbase)
{
    auto getAttribute = [&attr](QStringList &el){
        int listnummax = el.count();
        for(int pos = 2; pos < listnummax; pos += 2){
            if(attr == el.at(pos)){
                return el.at(pos + 1);
            }
        }

        return QString("");
    };

    for(QStringList element : *loadbase){
        if(tag == element.at(0)){
            if(attr == "") return element.at(1);
            return getAttribute(element);
        }
    }

    // not found
    return "";
}
