/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SWAPVARIANTTREE_H
#define SWAPVARIANTTREE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class SwapLocalVariantTree : public QUndoCommand
{
public:
    SwapLocalVariantTree(const int &localindex
                         , const int &before
                         , const int &after
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

#endif // SWAPVARIANTTREE_H
