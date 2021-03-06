/*
 * Copyright 2016-2021 karakirimu
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
