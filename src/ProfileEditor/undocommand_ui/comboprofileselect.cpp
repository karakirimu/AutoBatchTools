/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "comboprofileselect.h"

ComboProfileSelect::ComboProfileSelect(const int &targetindex
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

    oldFile = cache->at(index).profileload.filePath;
}

void ComboProfileSelect::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.profileload.filePath = oldFile;
    ptrCache->replace(index, ec);

    setText(QObject::tr("ProfileLoad '%1' selected").arg(newString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void ComboProfileSelect::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.profileload.filePath = newFile;
    ptrCache->replace(index, ec);

    setText(QObject::tr("ProfileLoad '%1' selected").arg(newString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

/**
 * @fn ComboProfileSelect::id
 * @brief Set id of back / forward command
 * @return Id (constant) of this back / forward command
 */
int ComboProfileSelect::id() const
{
    return UiCommandMap::PLOAD_FILEPATH;
}

bool ComboProfileSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboProfileSelect *com = static_cast<const ComboProfileSelect*>(other);
    newString = com->newString;
    newFile = com->newFile;
    return true;
}
