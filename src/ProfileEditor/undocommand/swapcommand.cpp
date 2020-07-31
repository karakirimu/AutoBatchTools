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

#include "swapcommand.h"

SwapCommand::SwapCommand(const int &beforeindex
                         , const int &afterindex
                         , EditorCacheList *cache
                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    beforeIndex = beforeindex;
    afterIndex = afterindex;
    ptrCache = cache;
}

void SwapCommand::undo()
{
    if(ptrCache->isEmpty()) return;
    ptrCache->move(afterIndex, beforeIndex);

    setText(QString("Swap item at No. %1 and No. %2").arg(beforeIndex).arg(afterIndex) \
            + QString(" ^(%1,%2,%3)").arg(afterIndex).arg(beforeIndex).arg(CommandMap::UNDOREDO_SWAP));
}

void SwapCommand::redo()
{
    if(ptrCache->isEmpty()) return;
    ptrCache->move(beforeIndex, afterIndex);

    setText(QString("Swap item at No. %1 and No. %2 ").arg(beforeIndex).arg(afterIndex) \
            + QString(" ^(%1,%2,%3)").arg(beforeIndex).arg(afterIndex).arg(CommandMap::UNDOREDO_SWAP));
}
