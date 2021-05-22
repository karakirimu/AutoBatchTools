/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringprojectdescription.h"

StringProjectDescription::StringProjectDescription(const int &targetindex
                                                   , const QString &newstring
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.description;
}

void StringProjectDescription::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.description = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Description changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringProjectDescription::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.description = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Description changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringProjectDescription::id() const
{
    return UiCommandMap::I_DESCRIPTION;
}

bool StringProjectDescription::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectDescription *com = static_cast<const StringProjectDescription*>(other);
    newStr = com->newStr;
    return true;
}
