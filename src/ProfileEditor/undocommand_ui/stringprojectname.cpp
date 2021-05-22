/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
