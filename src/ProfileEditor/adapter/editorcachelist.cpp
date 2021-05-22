/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editorcachelist.h"

EditorCacheList::EditorCacheList()
{
    converter = new EditorCacheConverter();
}

EditorCacheList::~EditorCacheList()
{
    delete converter;
}

/**
 * @fn EditorCacheList::serialize
 * @brief Convert to the XML output structure
 * @param dest Output destination
 */
void EditorCacheList::serialize(QList<QList<QStringList> *> *dest)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter->convertToXml(&cache, dest);
}

/**
 * @fn EditorCacheList::deSerialize
 * @brief Populate this list from an XML structure
 * @param source Input source
 */
void EditorCacheList::deSerialize(QList<QList<QStringList> *> *source)
{
    std::lock_guard<std::mutex> lock(mutex);
    converter->convertToEditorCache(source, &cache);
}

/**
 * @brief EditorCacheList::type
 * @param i Index
 * @return Selected type
 */
FunctionType::TYPE EditorCacheList::loadType(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    FunctionType ft;
    return ft.getType(cache.at(i).type);
}

const EditorCache EditorCacheList::at(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.at(i);
}

void EditorCacheList::append(const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.append(value);
}

void EditorCacheList::clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.clear();
}

void EditorCacheList::insert(int i, const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.insert(i, value);
}

bool EditorCacheList::isEmpty()
{
    std::lock_guard<std::mutex> lock(mutex);
    return cache.isEmpty();
}

void EditorCacheList::move(int from, int to)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.move(from, to);
}

void EditorCacheList::prepend(const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.prepend(value);
}

void EditorCacheList::removeAt(int i)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeAt(i);
}

void EditorCacheList::removeFirst()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeFirst();
}

void EditorCacheList::removeLast()
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.removeLast();
}

void EditorCacheList::replace(int i, const EditorCache &value)
{
    std::lock_guard<std::mutex> lock(mutex);
    cache.replace(i, value);
}

int EditorCacheList::count()
{
    int result = 0;
    {
        std::lock_guard<std::mutex> lock(mutex);
        result = cache.count();
    }
    return result;
}
