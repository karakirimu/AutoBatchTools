/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkeditrundetach.h"

CheckEditRunDetach::CheckEditRunDetach(const int &targetindex
                                       , const bool &newcheck
                                       , EditorCacheList *cache
                                       , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).exec.detach;
}

void CheckEditRunDetach::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.detach = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Wait for execution to end: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditRunDetach::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.detach = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Wait for execution to end: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditRunDetach::id() const
{
    return UiCommandMap::E_DETACH_BOOL;
}
