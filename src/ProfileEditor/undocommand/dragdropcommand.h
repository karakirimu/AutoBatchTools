/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef DRAGDROPCOMMAND_H
#define DRAGDROPCOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
#include <editorcachelist.h>

class DragDropCommand : public QUndoCommand
{
public:
    DragDropCommand(const QList<int> &beforeindex
                    , const int &afterindex
                    , EditorCacheList *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<int> beforeIndex;
    int afterIndex;

    EditorCacheList *ptrCache;
};

#endif // DRAGDROPCOMMAND_H
