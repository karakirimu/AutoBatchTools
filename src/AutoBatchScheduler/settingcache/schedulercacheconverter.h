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

#ifndef SCHEDULERCACHECONVERTER_H
#define SCHEDULERCACHECONVERTER_H

#include "schedulercache.h"
#include "SchedulerXmlConstant.h"
#include "../variantconverter/variantconverter.h"

class SchedulerCacheConverter
{
public:
    SchedulerCacheConverter();
    ~SchedulerCacheConverter();

    void convertToSchedulerCache(const QList<QList<QStringList> *> *source, QList<SchedulerCache> *dest);
    void convertToXml(const QList<SchedulerCache> *source, QList<QList<QStringList> *> *dest);

    void convertFromCache(const SchedulerCache *from, QList<QStringList> *to);
    void convertToCache(SchedulerCache *to, const QList<QStringList> *from);

private:

    QString fetch(QString tag, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);

    //! Date format
    const QString DATE_FORMAT = "yyyy/MM/dd HH:mm:ss";

    //! Time format
    const QString TIME_FORMAT = "HH:mm:ss";

    SchedulerXmlConstant sxc;
};

#endif // SCHEDULERCACHECONVERTER_H
