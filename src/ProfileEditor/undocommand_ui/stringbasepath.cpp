/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringbasepath.h"

StringBasePath::StringBasePath(const int &targetindex
                               , const QString &newstring
                               , EditorCacheList *cache
                               , QUndoCommand *parent)
     : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.basefilepath;
}

void StringBasePath::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.basefilepath = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change the working directory") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringBasePath::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.basefilepath = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change the working directory") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringBasePath::id() const
{
    return UiCommandMap::I_PROFILE_BASEPATH;
}

bool StringBasePath::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringBasePath *com = static_cast<const StringBasePath*>(other);
    newStr = com->newStr;
    return true;
}
