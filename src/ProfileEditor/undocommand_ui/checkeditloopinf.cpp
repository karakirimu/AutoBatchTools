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

#include "checkeditloopinf.h"

//CheckEditLoopInf::CheckEditLoopInf(const int &targetindex
//                                   , const bool &newcheck
//                                   , QList<QList<QStringList> *> *cache
//                                   , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    m_newcheck = VariantConverter::boolToString(newcheck);
//    m_cache = cache;

//    m_oldcheck = pxlg.fetch(I_RECURSIVE_LOOP, ATTR_NONE, m_cache->at(index));
//}

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
    // @deprecated
//    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_NONE, index, m_oldcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.processAll = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Loop infinity %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void CheckEditLoopInf::redo()
{
    // @deprecated
//    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_NONE, index, m_newcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.processAll = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Loop infinity %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}
