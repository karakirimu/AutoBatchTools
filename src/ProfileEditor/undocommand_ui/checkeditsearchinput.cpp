/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkeditsearchinput.h"

CheckEditSearchInput::CheckEditSearchInput(const int &targetindex
                                           , const bool &newcheck
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).info.fileInput;
}

void CheckEditSearchInput::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInputSearch = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("File input by search: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditSearchInput::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInputSearch = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("File input by search: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditSearchInput::id() const
{
    return UiCommandMap::I_FILEINPUT_SEARCH_BOOL;
}
