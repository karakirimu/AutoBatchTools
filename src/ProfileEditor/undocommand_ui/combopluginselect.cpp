/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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

    setText(QObject::tr("Plugin '%1' selected").arg(oldString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void ComboPluginSelect::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.plugin.name = newString;
    ec.plugin.filePath = newFile;
    ec.plugin.command.clear();
    ptrCache->replace(index, ec);

    setText(QObject::tr("Plugin '%1' selected").arg(newString) \
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
