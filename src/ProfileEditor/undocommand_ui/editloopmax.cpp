/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editloopmax.h"

EditLoopMax::EditLoopMax(const int &targetindex
                         , const int &newvalue
                         , EditorCacheList *cache
                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).info.processMaxCount;

}

void EditLoopMax::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.processMaxCount = oldValue;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Maximum number of loops: %1").arg(newValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditLoopMax::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.processMaxCount = newValue;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Maximum number of loops: %1").arg(oldValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditLoopMax::id() const
{
    return UiCommandMap::I_PROCESSMAX_INT;
}

bool EditLoopMax::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopMax *com = static_cast<const EditLoopMax *>(other);
    newValue = com->newValue;
    return true;
}
