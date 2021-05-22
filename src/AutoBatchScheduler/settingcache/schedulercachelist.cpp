/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
