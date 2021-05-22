/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringprojectauthor.h"

StringProjectAuthor::StringProjectAuthor(const int &targetindex
                                         , const QString &newstring
                                         , EditorCacheList *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(targetindex).info.author;
}

void StringProjectAuthor::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.author = oldStr;
    ptrCache->replace(index, ec);
    setText(QObject::tr("Author changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringProjectAuthor::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.author = newStr;
    ptrCache->replace(index, ec);
    setText(QObject::tr("Author changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringProjectAuthor::id() const
{
    return UiCommandMap::I_AUTHOR;
}

bool StringProjectAuthor::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectAuthor *com = static_cast<const StringProjectAuthor*>(other);
    newStr = com->newStr;
    return true;
}
