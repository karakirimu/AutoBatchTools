/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "insertcommand.h"

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
    if(ptrCache->isEmpty()) return;
    ptrCache->removeAt(index);

    setText(QObject::tr("Insert item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_DELETE));
}

void InsertCommand::redo()
{
    ptrCache->insert(index, insertCache);

    setText(QObject::tr("Insert item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_INSERT));
}
