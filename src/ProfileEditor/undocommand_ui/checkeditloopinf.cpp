/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "checkeditloopinf.h"

CheckEditLoopInf::CheckEditLoopInf(const int &targetindex
                                   , const bool &newcheck
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newCheck = newcheck;
    ptrCache = cache;
    oldCheck = cache->at(index).info.processAll;
}

void CheckEditLoopInf::undo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.processAll = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Process all entered files: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void CheckEditLoopInf::redo()
{
    EditorCache ec = ptrCache->at(index);
    ec.info.processAll = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Process all entered files: %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}
