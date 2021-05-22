/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITCOMBOBOXCOMMAND_H
#define EDITCOMBOBOXCOMMAND_H

//for localvariable combobox
#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditComboBoxCommand : public QUndoCommand
{
public:
    EditComboBoxCommand(const int &targetindex
                        , const QString newstring
                        , EditorCacheList *cache
                        , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

private:
    int index;
    QString oldString;
    QString newString;

    EditorCacheList *ptrCache;

};

#endif // EDITCOMBOBOXCOMMAND_H
