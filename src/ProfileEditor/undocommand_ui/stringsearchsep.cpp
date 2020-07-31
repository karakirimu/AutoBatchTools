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

#include "stringsearchsep.h"

StringSearchSep::StringSearchSep(const int &targetindex
                                 , const QString &newstring
                                 , EditorCacheList *cache
                                 , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).filesearch.separator;
}

void StringSearchSep::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.separator = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Search delimiter changed to '%1' at No. %2").arg(newStr).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringSearchSep::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.separator = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Search delimiter changed to '%1' at No. %2").arg(oldStr).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringSearchSep::id() const
{
    return UiCommandMap::FS_SEPARATOR;
}

bool StringSearchSep::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringSearchSep *com = static_cast<const StringSearchSep*>(other);
    newStr = com->newStr;
    return true;
}
