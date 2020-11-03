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
    SchedulerCacheConverter *converter;
};

#endif // SCHEDULERCACHELIST_H
