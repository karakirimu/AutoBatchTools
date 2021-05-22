/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editcomboboxcommand.h"

EditComboBoxCommand::EditComboBoxCommand(const int &targetindex
                                         , const QString newstring
                                         , EditorCacheList *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newString = newstring;
    ptrCache = cache;

    oldString = cache->at(index).filesearch.variant;
}

void EditComboBoxCommand::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.variant = oldString;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Set the return variable of No.%1 to '%2'").arg(index).arg(oldString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditComboBoxCommand::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.variant = newString;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Set the return variable of No.%1 to '%2'").arg(index).arg(newString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditComboBoxCommand::id() const
{
    return UiCommandMap::FS_VARIANT;
}

bool EditComboBoxCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditComboBoxCommand *com = static_cast<const EditComboBoxCommand*>(other);
    newString = com->newString;
    return true;
}
