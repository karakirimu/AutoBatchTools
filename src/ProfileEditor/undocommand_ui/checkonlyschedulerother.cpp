/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkonlyschedulerother.h"

CheckOnlySchedulerOther::CheckOnlySchedulerOther(const int &targetindex
                                                 , const bool &newcheck
                                                 , EditorCacheList *cache
                                                 , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).profileload.schedulerOnly;
}

void CheckOnlySchedulerOther::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.profileload.schedulerOnly = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckOnlySchedulerOther::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.profileload.schedulerOnly = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckOnlySchedulerOther::id() const
{
    return UiCommandMap::PR_ONLY_SCHEDULER;
}
