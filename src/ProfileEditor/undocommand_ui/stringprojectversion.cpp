/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringprojectversion.h"

StringProjectVersion::StringProjectVersion(const int &targetindex
                                           , const QString &newstring
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.version;
}

void StringProjectVersion::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.version = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Version changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringProjectVersion::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.version = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Version changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringProjectVersion::id() const
{
    return UiCommandMap::I_VERSION;
}

bool StringProjectVersion::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectVersion *com = static_cast<const StringProjectVersion*>(other);
    newStr = com->newStr;
    return true;
}
