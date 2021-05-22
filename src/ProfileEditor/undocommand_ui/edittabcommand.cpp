/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "edittabcommand.h"

EditTabCommand::EditTabCommand(const int &targetindex
                               , const int &newindex
                               , EditorCacheList *cache
                               , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newIndex = newindex;
    ptrCache = cache;
    oldType = cache->at(index).type;
    oldIndex = cache->at(index).functionSelect;
}

void EditTabCommand::undo()
{    
    EditorCache ec = ptrCache->at(index);
    FunctionType ft;
    ec.type = oldType;
    ec.functionSelect = oldIndex;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change No. %1 execution type").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditTabCommand::redo()
{
    EditorCache ec = ptrCache->at(index);
    FunctionType ft;
    if(ec.type != ft.getString(ft.TYPE::ALLINCLUDE)){
        ec.type = ft.getString(ft.TYPE::ALLINCLUDE);
    }
    ec.functionSelect = newIndex;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change No. %1 execution type").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditTabCommand::id() const
{
    return UiCommandMap::FUNCTIONSELECT;
}
