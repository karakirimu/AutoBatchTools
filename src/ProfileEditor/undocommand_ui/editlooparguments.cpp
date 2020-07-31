/*
 * Copyright 2016-2020 karakirimu
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
