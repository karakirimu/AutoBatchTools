/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkonlyschedulerplugin.h"

CheckOnlySchedulerPlugin::CheckOnlySchedulerPlugin(const int &targetindex
                                                   , const bool &newcheck
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).plugin.schedulerOnly;
}

void CheckOnlySchedulerPlugin::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.plugin.schedulerOnly = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckOnlySchedulerPlugin::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.plugin.schedulerOnly = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only: %1 at No. %2").arg(newCheck).arg(index) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int CheckOnlySchedulerPlugin::id() const
{
    return UiCommandMap::PL_ONLY_SCHEDULER;
}
