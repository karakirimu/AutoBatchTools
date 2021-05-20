/*
 * Copyright 2016-2021 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
