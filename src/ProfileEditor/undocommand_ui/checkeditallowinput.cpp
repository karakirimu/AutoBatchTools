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

#include "checkeditallowinput.h"

CheckEditAllowInput::CheckEditAllowInput(const int &targetindex
                                         , const bool &newcheck
                                         , EditorCacheList *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).info.fileInput;
}

void CheckEditAllowInput::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInput = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Allow input %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditAllowInput::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInput = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Allow input %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditAllowInput::id() const
{
    return UiCommandMap::I_FILEINPUT_BOOL;
}
