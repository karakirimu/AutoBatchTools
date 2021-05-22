/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "addcommand.h"

AddCommand::AddCommand(const int &targetindex
                       , EditorCacheList *cache
                       , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    ptrCache = cache;
}

void AddCommand::undo()
{
    if(ptrCache->isEmpty()) return;
    ptrCache->removeAt(index);

    setText(QObject::tr("Add item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_DELETE));
}

void AddCommand::redo()
{
    EditorCache add;
    // init
    FunctionType ft;
    add.type = ft.getString(ft.TYPE::ALLINCLUDE);
    add.functionSelect = static_cast<int>(TAB::EXECUTE);

    ptrCache->append(add);

    setText(QObject::tr("Add item at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(CommandMap::UNDOREDO_ADD));
}
