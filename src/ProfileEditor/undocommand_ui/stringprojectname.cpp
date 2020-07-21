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

#include "stringprojectname.h"

StringProjectName::StringProjectName(const int &targetindex
                                     , const QString &newstring
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
     : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.name;
}

void StringProjectName::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.name = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Profile name changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringProjectName::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.name = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Profile name changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringProjectName::id() const
{
    return UiCommandMap::I_NAME;
}

bool StringProjectName::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectName *com = static_cast<const StringProjectName*>(other);
    newStr = com->newStr;
    return true;
}
