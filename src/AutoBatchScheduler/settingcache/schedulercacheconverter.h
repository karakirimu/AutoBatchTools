/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
