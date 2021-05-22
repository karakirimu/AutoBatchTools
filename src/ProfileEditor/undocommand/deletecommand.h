/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
#include <editorcachelist.h>

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(const int &targetindex
                  , const EditorCache &remove
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;

    EditorCache removeCache;
    EditorCacheList *ptrCache;
};

#endif // DELETECOMMAND_H
