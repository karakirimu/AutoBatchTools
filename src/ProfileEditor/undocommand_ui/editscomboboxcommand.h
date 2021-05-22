/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITSCOMBOBOXCOMMAND_H
#define EDITSCOMBOBOXCOMMAND_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

//for search combobox
class EditScomboBoxCommand : public QUndoCommand
{
public:
    EditScomboBoxCommand(const int &targetindex
                         , const QString &newstring
                         , const int &newsearchindex
                         , EditorCacheList *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
private:
    void replace(QString str, int i);

    int index;
    QString oldString;
    QString newString;
    int oldIndex;
    int newIndex;

    FunctionType::TYPE select;

    EditorCacheList *ptrCache;
};

#endif // EDITSCOMBOBOXCOMMAND_H
