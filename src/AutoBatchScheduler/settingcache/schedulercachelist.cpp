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

#include "schedulercachelist.h"

SchedulerCacheList::SchedulerCacheList()
{
}

SchedulerCacheList::~SchedulerCacheList()
{
}

/**
 * @fn SchedulerCacheList::serialize
 * @brief Convert to the XML output structure
 * @param dest Output destination
 */
void SchedulerCacheList::serialize(QList<QList<QStringList> *> *dest)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter.convertToXml(&cache, dest);
}

/**
 * @fn SchedulerCacheList::deSerialize
 * @brief Populate this list from an XML structure
 * @param source Input source
 */
void SchedulerCacheList::deSerialize(QList<QList<QStringList> *> *source)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter.convertToSchedulerCache(source, &cache);
}

const SchedulerCache SchedulerCacheList::at(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.at(i);
}

void SchedulerCacheList::append(const SchedulerCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.append(value);
}

void SchedulerCacheList::clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.clear();
}

void SchedulerCacheList::insert(int i, const SchedulerCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.insert(i, value);
}

bool SchedulerCacheList::isEmpty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.isEmpty();
}

void SchedulerCacheList::move(int from, int to)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.move(from, to);
}

void SchedulerCacheList::prepend(const SchedulerCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.prepend(value);
}

void SchedulerCacheList::removeAt(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeAt(i);
}

void SchedulerCacheList::removeFirst()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeFirst();
}

void SchedulerCacheList::removeLast()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeLast();
}

void SchedulerCacheList::replace(int i, const SchedulerCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.replace(i, value);
}

int SchedulerCacheList::count()
{
    int result = 0;
    {
        std::lock_guard<std::mutex> lock(mutex);
        result = cache.count();
    }
    return result;
}
