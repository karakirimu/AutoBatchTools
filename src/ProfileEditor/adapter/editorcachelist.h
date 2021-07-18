/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
    void serializeForExport(QList<QList<QStringList> *> *dest);
    void deSerialize(QList<QList<QStringList> *> *cache);
    FunctionType::TYPE loadType(int i);

    const EditorCache at(int i);

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

    const QList<EditorCache> getCacheCopy(){ return cache; }

private:

    std::mutex mutex;
    QList<EditorCache> cache;
    EditorCacheConverter *converter;

};

#endif // EDITORCACHEMANAGER_H
