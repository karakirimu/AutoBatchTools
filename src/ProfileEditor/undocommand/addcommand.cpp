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
