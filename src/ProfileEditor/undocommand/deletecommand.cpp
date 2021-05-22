/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "deletecommand.h"

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
    ptrCache->insert(index, removeCache);

    setText(QObject::tr("Remove item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_INSERT));
}

void DeleteCommand::redo()
{
    if(ptrCache->isEmpty()) return;
    ptrCache->removeAt(index);

    setText(QObject::tr("Remove item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_DELETE));
}
