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

#include "deletecommand.h"

//DeleteCommand::DeleteCommand(const int &targetindex
//                             , QList<QStringList> *deleted
//                             , QList<QList<QStringList> *> *cache
//                             , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
////    m_deleted = deleted;
//    m_delcopy.append(*deleted);
//    m_cache = cache;

//}

DeleteCommand::DeleteCommand(const int &targetindex
                             , const EditorCache &remove
                             , EditorCacheList *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    index = targetindex;
    removeCache = remove;
    ptrCache = cache;
}

void DeleteCommand::undo()
{
    // @deprecated
//    {
////    m_deleted = new QList<QStringList>(m_delcopy);
//    m_cache->insert(index, new QList<QStringList>(m_delcopy));
//    }

    ptrCache->insert(index, removeCache);

    setText(QObject::tr("Remove item at %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_INSERT));
}

void DeleteCommand::redo()
{
    // @deprecated
//    {
//    if(m_cache->isEmpty()) return;

////    delete m_deleted;
//    m_cache->removeAt(index);
//    }

    if(ptrCache->isEmpty()) return;
    ptrCache->removeAt(index);

    setText(QObject::tr("Remove item at %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_DELETE));
}
