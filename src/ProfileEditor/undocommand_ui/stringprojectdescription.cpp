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

#include "stringprojectdescription.h"

//StringProjectDescription::StringProjectDescription(const int &targetindex
//                                                   , QString newstring
//                                                   , QList<QList<QStringList> *> *cache
//                                                   , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newStr = newstring;

//    m_cache = cache;
//    oldStr = pxlg.fetch(I_DESCRIPTION, ATTR_NONE, m_cache->at(index));
//}

StringProjectDescription::StringProjectDescription(const int &targetindex
                                                   , const QString &newstring
                                                   , EditorCacheList *cache
                                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.description;
}

void StringProjectDescription::undo()
{
    // @deprecated
//    pxlg.replaceElementList(I_DESCRIPTION, ATTR_NONE, index, oldStr, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.description = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Description Text") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void StringProjectDescription::redo()
{
    // @deprecated
//    pxlg.replaceElementList(I_DESCRIPTION, ATTR_NONE, index, newStr, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.description = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Description Text") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int StringProjectDescription::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_DESCRIPTION);
}

bool StringProjectDescription::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectDescription *com = static_cast<const StringProjectDescription*>(other);
    newStr = com->newStr;
    return true;
}
