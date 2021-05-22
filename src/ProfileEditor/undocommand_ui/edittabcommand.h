/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITTABCOMMAND_H
#define EDITTABCOMMAND_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class EditTabCommand : public QUndoCommand
{
public:
    EditTabCommand(const int &targetindex
                   , const int &newindex
                   , EditorCacheList *cache
                   , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    int index;
    int oldIndex;
    QString oldType;
    int newIndex;

    EditorCacheList *ptrCache;

};

#endif // EDITTABCOMMAND_H
