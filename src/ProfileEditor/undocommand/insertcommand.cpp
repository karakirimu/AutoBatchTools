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

#include "insertcommand.h"

//InsertCommand::InsertCommand(const int &targetindex
//                             , QList<QStringList> *inserted
//                             , QList<QList<QStringList> *> *cache
//                             , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
////    m_inserted = inserted;
//    m_inscopy.append(*inserted);
//    m_cache = cache;
//}

InsertCommand::InsertCommand(const int &targetindex
                             ,const EditorCache &insert
                             , EditorCacheList *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    index = targetindex;
    insertCache = insert;
    ptrCache = cache;
}

void InsertCommand::undo()
{
    // @deprecated
//    {
//    if(m_cache->isEmpty()) return;
////    delete m_inserted;
//    m_cache->removeAt(index);
//    }

    if(ptrCache->isEmpty()) return;
    ptrCache->removeAt(index);

    setText(QObject::tr("Insert item at %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_DELETE));
}

void InsertCommand::redo()
{
    // @deprecated
//    {
////    m_inserted = new QList<QStringList>(m_inscopy);
//    m_cache->insert(index, new QList<QStringList>(m_inscopy));
//    }

    ptrCache->insert(index, insertCache);

    setText(QObject::tr("Insert item at %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_INSERT));
}
