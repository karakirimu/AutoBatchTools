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

#include "checkedittimeout.h"

CheckEditTimeout::CheckEditTimeout(const int &targetindex
                                   , const bool &newcheck
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).exec.timeoutEnabled;
}

void CheckEditTimeout::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditTimeout::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditTimeout::id() const
{
    return UiCommandMap::E_TIMEOUT_BOOL_HA1;
}
