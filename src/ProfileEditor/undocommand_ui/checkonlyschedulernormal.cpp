/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkonlyschedulernormal.h"

CheckOnlySchedulerNormal::CheckOnlySchedulerNormal(const int &targetindex
                                                   , const bool &newcheck
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).exec.schedulerOnly;
}

void CheckOnlySchedulerNormal::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.schedulerOnly = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckOnlySchedulerNormal::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.exec.schedulerOnly = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckOnlySchedulerNormal::id() const
{
    return UiCommandMap::E_ONLY_SCHEDULER;
}
