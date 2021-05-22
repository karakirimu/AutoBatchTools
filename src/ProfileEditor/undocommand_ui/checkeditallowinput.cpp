/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkeditallowinput.h"

CheckEditAllowInput::CheckEditAllowInput(const int &targetindex
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

void CheckEditAllowInput::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInput = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Allow file input: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditAllowInput::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.fileInput = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Allow file input: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditAllowInput::id() const
{
    return UiCommandMap::I_FILEINPUT_BOOL;
}
