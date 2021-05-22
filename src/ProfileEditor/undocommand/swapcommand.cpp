/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
