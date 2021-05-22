/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editsearchoutput.h"

EditSearchOutput::EditSearchOutput(const int &targetindex
                                   , const int &newvalue
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).filesearch.outputOption;
}

void EditSearchOutput::undo()
{
    replaceValue(oldValue);

    setText(QObject::tr("Output method of filesearch changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditSearchOutput::redo()
{
    replaceValue(newValue);

    setText(QObject::tr("Output method of filesearch changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditSearchOutput::id() const
{
    return UiCommandMap::FS_OUTPUTOPTION_INT;
}

bool EditSearchOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchOutput *com = static_cast<const EditSearchOutput *>(other);
    newValue = com->newValue;
    return true;
}

void EditSearchOutput::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.outputOption = value;
    ptrCache->replace(index, ec);
}
