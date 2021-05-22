/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringfileoutput.h"

StringFileOutput::StringFileOutput(const int &targetindex
                                   , const QString &newstring
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).filesearch.filePath;
}

void StringFileOutput::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.filePath = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change filesearch output at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void StringFileOutput::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.filePath = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Change filesearch output at No. %1").arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int StringFileOutput::id() const
{
    return UiCommandMap::FS_FILEPATH_HA1;
}

bool StringFileOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringFileOutput *com = static_cast<const StringFileOutput*>(other);
    newStr = com->newStr;
    return true;
}
