/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SCHEDULERCACHELIST_H
#define SCHEDULERCACHELIST_H

#include <mutex>
#include "schedulercache.h"
#include "schedulercacheconverter.h"

class SchedulerCacheList
{
public:
    SchedulerCacheList();
    ~SchedulerCacheList();

    void serialize(QList<QList<QStringList> *> *cache);
    void deSerialize(QList<QList<QStringList> *> *cache);

    const SchedulerCache at(int i);

    void append(const SchedulerCache &value);

    void clear();
    int count();

    void insert(int i, const SchedulerCache &value);

    bool isEmpty();

    void move(int from, int to);

    void prepend(const SchedulerCache &value);

    void removeAt(int i);
    void removeFirst();
    void removeLast();

    void replace(int i, const SchedulerCache &value);

    const QList<SchedulerCache> getCacheCopy(){ return cache; }

private:

    std::mutex mutex;
    QList<SchedulerCache> cache;
    SchedulerCacheConverter converter;
};

#endif // SCHEDULERCACHELIST_H
