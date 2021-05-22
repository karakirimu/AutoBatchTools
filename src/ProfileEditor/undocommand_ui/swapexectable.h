/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SWAPEXECTABLE_H
#define SWAPEXECTABLE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class SwapExecTable : public QUndoCommand
{
public:
    SwapExecTable(const int &targetindex
                  , const int &tablebefore
                  , const int &tableafter
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    int id() const override;

private:
    void replaceValue(int source, int dest);
    void setTextByCondition(int source, int dest);

    int index;
    int indexBefore;
    int indexAfter;

    EditorCacheList *ptrCache;
};

#endif // SWAPEXECTABLE_H
