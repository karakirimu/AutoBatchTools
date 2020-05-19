/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "checkonlyschedulernormal.h"

//CheckOnlySchedulerNormal::CheckOnlySchedulerNormal(const int &targetindex
//                                                   , const bool &newcheck
//                                                   , QList<QList<QStringList> *> *cache
//                                                   , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    m_newcheck = VariantConverter::boolToString(newcheck);
//    m_cache = cache;

//    m_oldcheck = pxlg.fetch(ALL_TYPE, TYPE_EXEC, ATTR_ONLY_SCHEDULER, m_cache->at(index));
//}

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
    // @deprecated
//    pxlg.replaceTypeElement(TYPE_EXEC, ATTR_ONLY_SCHEDULER, index, m_oldcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.exec.schedulerOnly = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void CheckOnlySchedulerNormal::redo()
{
    // @deprecated
//    pxlg.replaceTypeElement(TYPE_EXEC, ATTR_ONLY_SCHEDULER, index, m_newcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.exec.schedulerOnly = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Scheduler only %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int CheckOnlySchedulerNormal::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_ONLY_SCHEDULER);
}
