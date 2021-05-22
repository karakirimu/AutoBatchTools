/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "edittimeout.h"

EditTimeout::EditTimeout(const int &targetindex
                            , const int &newvalue
                            , EditorCacheList *cache
                            , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).exec.timeout;
}

void EditTimeout::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeout = oldValue;
    ptrCache->replace(index, ec);

    setText(QObject::tr("No. %1 timeout changed to %2 ms").arg(index).arg(newValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditTimeout::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeout = newValue;
    ptrCache->replace(index, ec);

    setText(QObject::tr("No. %1 timeout changed to %2 ms").arg(index).arg(oldValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditTimeout::id() const
{
    return UiCommandMap::E_TIMEOUT_INT;
}

bool EditTimeout::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTimeout *com = static_cast<const EditTimeout *>(other);
    newValue = com->newValue;
    return true;
}
