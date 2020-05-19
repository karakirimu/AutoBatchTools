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

#include "checkedittimeout.h"

//CheckEditTimeout::CheckEditTimeout(const int &targetindex
//                                         , const bool &newcheck
//                                         , QList<QList<QStringList> *> *cache
//                                         , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    m_newcheck = VariantConverter::boolToString(newcheck);
//    m_cache = cache;

//    m_oldcheck = pxlg.fetch(E_TIMEOUT, ATTR_NONE, m_cache->at(index));
//}

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
    // @deprecated
//    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, index, m_oldcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = oldCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void CheckEditTimeout::redo()
{
    // @deprecated
//    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, index, m_newcheck, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.exec.timeoutEnabled = newCheck;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Timeout %1").arg(newCheck) \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int CheckEditTimeout::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_TIMEOUT);
}
