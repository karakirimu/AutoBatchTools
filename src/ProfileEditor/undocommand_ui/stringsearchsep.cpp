/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
