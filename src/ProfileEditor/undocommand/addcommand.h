/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
#include <editorcachelist.h>

class AddCommand : public QUndoCommand
{
public:
    AddCommand(const int &targetindex
               , EditorCacheList *cache
               , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;

    EditorCacheList *ptrCache;
};

#endif // ADDCOMMAND_H
