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

#include "combopluginselect.h"

ComboPluginSelect::ComboPluginSelect(const int &targetindex
                                     , const QString newstring
                                     , const QString newfile
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newString = newstring;
    ptrCache = cache;
    newFile = newfile;

    oldString = cache->at(index).plugin.name;
    oldFile = cache->at(index).plugin.filePath;
    oldCommand = cache->at(index).plugin.command;
}

void ComboPluginSelect::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.plugin.name = oldString;
    ec.plugin.filePath = oldFile;
    ec.plugin.command = oldCommand;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Plugins to %1").arg(oldString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void ComboPluginSelect::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.plugin.name = newString;
    ec.plugin.filePath = newFile;
    ec.plugin.command.clear();
    ptrCache->replace(index, ec);

    setText(QObject::tr("Plugins to %1").arg(newString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int ComboPluginSelect::id() const
{
    return UiCommandMap::P_NAME;
}

//bool ComboPluginSelect::mergeWith(const QUndoCommand *other)
//{
//    if (other->id() != id()) return false;
//    const ComboPluginSelect *com = static_cast<const ComboPluginSelect*>(other);
//    newString = com->newString;
//    newFile = com->newFile;
//    return true;
//}
