/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editsearchfileoutputtype.h"

EditSearchFileOutputType::EditSearchFileOutputType(const int &targetindex
                                                   , const int &newvalue
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).filesearch.writeOption;
}

void EditSearchFileOutputType::undo()
{
    replaceValue(oldValue);

    setText(QObject::tr("Save mode changed at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditSearchFileOutputType::redo()
{
    replaceValue(newValue);

    setText(QObject::tr("Save mode changed at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditSearchFileOutputType::id() const
{
    return UiCommandMap::FS_WRITEOPTION_INT;
}

bool EditSearchFileOutputType::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchFileOutputType *com = static_cast<const EditSearchFileOutputType *>(other);
    newValue = com->newValue;
    return true;
}

void EditSearchFileOutputType::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.writeOption = value;
    ptrCache->replace(index, ec);
}
