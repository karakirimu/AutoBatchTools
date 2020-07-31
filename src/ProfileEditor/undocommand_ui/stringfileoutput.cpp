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

#include "stringfileoutput.h"

StringFileOutput::StringFileOutput(const int &targetindex
                                   , const QString &newstring
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).filesearch.filePath;
}

void StringFileOutput::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.filePath = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change filesearch output at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringFileOutput::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.filePath = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change filesearch output at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringFileOutput::id() const
{
    return UiCommandMap::FS_FILEPATH_HA1;
}

bool StringFileOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringFileOutput *com = static_cast<const StringFileOutput*>(other);
    newStr = com->newStr;
    return true;
}
