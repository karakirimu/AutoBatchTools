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

#include "editorcachelist.h"

EditorCacheList::EditorCacheList()
{
    cache = new QList<EditorCache>();
    converter = new EditorCacheConverter();
}

EditorCacheList::~EditorCacheList()
{
    delete converter;
    delete cache;
}

/**
 * @fn EditorCacheList::serialize
 * @brief Convert to the XML output structure
 * @param dest Output destination
 */
void EditorCacheList::serialize(QList<QList<QStringList> *> *dest)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter->convertToXml(cache, dest);
}

/**
 * @fn EditorCacheList::deSerialize
 * @brief Populate this list from an XML structure
 * @param source Input source
 */
void EditorCacheList::deSerialize(QList<QList<QStringList> *> *source)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter->convertToEditorCache(source, cache);
}

void EditorCacheList::append(const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->append(value);
}

void EditorCacheList::clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache->clear();
}

void EditorCacheList::insert(int i, const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->insert(i, value);
}

bool EditorCacheList::isEmpty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache->isEmpty();
}

void EditorCacheList::move(int from, int to)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->move(from, to);
}

void EditorCacheList::prepend(const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->prepend(value);
}

void EditorCacheList::removeAt(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->removeAt(i);
}

void EditorCacheList::removeFirst()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->removeFirst();
}

void EditorCacheList::removeLast()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->removeLast();
}

void EditorCacheList::replace(int i, const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache->replace(i, value);
}

int EditorCacheList::count()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache->count();
}
