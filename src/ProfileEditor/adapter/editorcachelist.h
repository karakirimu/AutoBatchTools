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

#ifndef EDITORCACHEMANAGER_H
#define EDITORCACHEMANAGER_H

#include <mutex>
#include "editorcache.h"
#include "editorcacheconverter.h"

/**
 * @brief The EditorCacheList
 *        Wrapper class for QList for EditorCache
 *        All acceptable processes have implemented std::lock_guard.
 */
class EditorCacheList
{
public:
    explicit EditorCacheList();
    ~EditorCacheList();

    void serialize(QList<QList<QStringList> *> *cache);
    void deSerialize(QList<QList<QStringList> *> *cache);

    void append(const EditorCache &value);

    void clear();
    int count();

    void insert(int i, const EditorCache &value);

    bool isEmpty();

    void move(int from, int to);

    void prepend(const EditorCache &value);

    void removeAt(int i);
    void removeFirst();
    void removeLast();

    void replace(int i, const EditorCache &value);


private:
    std::mutex mutex;
    QList<EditorCache> *cache;
    EditorCacheConverter *converter;

};

#endif // EDITORCACHEMANAGER_H
