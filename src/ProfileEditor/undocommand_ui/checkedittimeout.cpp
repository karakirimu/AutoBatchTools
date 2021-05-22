/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkedittimeout.h"

CheckEditTimeout::CheckEditTimeout(const int &targetindex
                                   , const bool &newcheck
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).exec.timeoutEnabled;
}

void CheckEditTimeout::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout option at No. %1: %2").arg(index).arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditTimeout::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout option at No. %1: %2").arg(index).arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckEditTimeout::id() const
{
    return UiCommandMap::E_TIMEOUT_BOOL_HA1;
}
