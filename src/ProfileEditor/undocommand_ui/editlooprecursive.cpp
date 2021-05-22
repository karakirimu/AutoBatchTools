/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editlooprecursive.h"

EditLoopRecursive::EditLoopRecursive(const int &targetindex
                                     , const int &newvalue
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).info.recursiveLoopMax;
}

void EditLoopRecursive::undo()
{
    replaceValue(oldValue);

    setText(QObject::tr("Number of recursive loops: %1").arg(newValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditLoopRecursive::redo()
{
    replaceValue(newValue);

    setText(QObject::tr("Number of recursive loops: %1").arg(oldValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditLoopRecursive::id() const
{
    return UiCommandMap::I_RECURSIVE_LOOPMAX_INT;
}

bool EditLoopRecursive::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopRecursive *com = static_cast<const EditLoopRecursive *>(other);
    newValue = com->newValue;
    return true;
}

void EditLoopRecursive::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.info.recursiveLoopMax = value;
    ptrCache->replace(index, ec);
}
