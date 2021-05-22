/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
#include <editorcachelist.h>

class InsertCommand : public QUndoCommand
{
public:
    InsertCommand(const int &targetindex
                  , const EditorCache &insert
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;

    EditorCache insertCache;
    EditorCacheList *ptrCache;
};

#endif // INSERTCOMMAND_H
