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

#include "editlooprecursive.h"

//EditLoopRecursive::EditLoopRecursive(const int &targetindex
//                                     , int newvalue
//                                     , QList<QList<QStringList> *> *cache
//                                     , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newValue = newvalue;
//    m_cache = cache;

//    oldValue = static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, m_cache->at(index))).toInt();

//}

EditLoopRecursive::EditLoopRecursive(const int &targetindex
                                     , const int &newvalue
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).info.recursiveLoopMax;
}

void EditLoopRecursive::undo()
{
    // @deprecated
//    {
//    pxlg.replaceElementList(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, index, QString::number(oldValue), m_cache);
//    }
    replaceValue(oldValue);

    setText(QObject::tr("Loop recursive count to %1").arg(newValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void EditLoopRecursive::redo()
{
    // @deprecated
//    {
//    pxlg.replaceElementList(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, index, QString::number(newValue), m_cache);
//    }

    replaceValue(newValue);

    setText(QObject::tr("Loop recursive count to %1").arg(oldValue) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

int EditLoopRecursive::id() const
{
//    ProcessXmlListGenerator pxg;
//    return pxg.getId(I_RECURSIVE_LOOPCOUNT);

    return UiCommandMap::I_RECURSIVE_LOOPMAX_INT;
}

bool EditLoopRecursive::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopRecursive *com = static_cast<const EditLoopRecursive *>(other);
    newValue = com->newValue;
    return true;
}

void EditLoopRecursive::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.info.recursiveLoopMax = value;
    ptrCache->replace(index, ec);
}
