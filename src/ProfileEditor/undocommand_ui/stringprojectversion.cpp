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

#include "stringprojectversion.h"

//StringProjectVersion::StringProjectVersion(const int &targetindex
//                                           , QString newstring
//                                           , QList<QList<QStringList> *> *cache
//                                           , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newStr = newstring;

//    m_cache = cache;
////    ProcessXmlListGenerator x;
////    x.getListStructure(cache->at(m_targetindex), &posinfo);

//    //info version
////    m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);
//    oldStr = pxlg.fetch(I_VERSION, ATTR_NONE, m_cache->at(index));

//}

StringProjectVersion::StringProjectVersion(const int &targetindex
                                           , const QString &newstring
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newStr = newstring;
    ptrCache = cache;
    oldStr = cache->at(index).info.version;
}

void StringProjectVersion::undo()
{
//    if(m_cache->isEmpty()) return;

//    QStringList alist = m_cache->at(m_targetindex)->at(2);
//    alist.replace(1, m_oldstring);
//    m_cache->at(m_targetindex)->replace(2, alist);
    // @deprecated
//    pxlg.replaceElementList(I_VERSION, ATTR_NONE, index, oldStr, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.version = oldStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Version changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void StringProjectVersion::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(2);
//    alist.replace(1, m_newstring);
//    m_cache->at(m_targetindex)->replace(2, alist);
    // @deprecated
//    pxlg.replaceElementList(I_VERSION, ATTR_NONE, index, newStr, m_cache);

    EditorCache ec = ptrCache->at(index);
    ec.info.version = newStr;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Version changed") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int StringProjectVersion::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_VERSION);
}

bool StringProjectVersion::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectVersion *com = static_cast<const StringProjectVersion*>(other);
    newStr = com->newStr;
    return true;
}
