/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SWAPCOMMAND_H
#define SWAPCOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
#include <editorcachelist.h>

class SwapCommand : public QUndoCommand
{
public:
    SwapCommand(const int &beforeindex
                , const int &afterindex
                , EditorCacheList *cache
                , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int beforeIndex;
    int afterIndex;

    EditorCacheList *ptrCache;
};

#endif // SWAPCOMMAND_H
