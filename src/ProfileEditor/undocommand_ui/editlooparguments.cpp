/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editlooparguments.h"

EditLoopArguments::EditLoopArguments(const int &targetindex
                                     , const int &newvalue
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).info.argumentsInOneLoop;
}

void EditLoopArguments::undo()
{
    replaceValue(oldValue);

    setText(QObject::tr("Number of File Input in one loop: %1").arg(newValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditLoopArguments::redo()
{
    replaceValue(newValue);

    setText(QObject::tr("Number of File Input in one loop: %1").arg(oldValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditLoopArguments::id() const
{
    return UiCommandMap::I_ARG_IN_ONELOOP_INT;
}

bool EditLoopArguments::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopArguments *com = static_cast<const EditLoopArguments *>(other);
    newValue = com->newValue;
    return true;
}

void EditLoopArguments::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.info.argumentsInOneLoop = value;
    ptrCache->replace(index, ec);
}
