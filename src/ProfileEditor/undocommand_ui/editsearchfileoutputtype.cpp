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

#include "editsearchfileoutputtype.h"

EditSearchFileOutputType::EditSearchFileOutputType(const int &targetindex
                                                   , const int &newvalue
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).filesearch.writeOption;
}

void EditSearchFileOutputType::undo()
{
    replaceValue(oldValue);

    setText(QObject::tr("Change output save mode") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditSearchFileOutputType::redo()
{
    replaceValue(newValue);

    setText(QObject::tr("Change output save mode") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditSearchFileOutputType::id() const
{
    return UiCommandMap::FS_WRITEOPTION_INT;
}

bool EditSearchFileOutputType::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchFileOutputType *com = static_cast<const EditSearchFileOutputType *>(other);
    newValue = com->newValue;
    return true;
}

void EditSearchFileOutputType::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.writeOption = value;
    ptrCache->replace(index, ec);
}
