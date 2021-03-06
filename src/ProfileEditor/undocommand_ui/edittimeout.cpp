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
