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

#include "comboprofileselect.h"

//ComboProfileSelect::ComboProfileSelect(const int &targetindex
//                                       , const QString newstring
//                                       , const QString newfile
//                                       , QList<QList<QStringList> *> *cache
//                                       , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newString = newstring;

//    m_cache = cache;
//    newFile = newfile;

//    oldFile = pxlg.fetch(PR_FILEPATH, ATTR_NONE, m_cache->at(index));

//}

ComboProfileSelect::ComboProfileSelect(const int &targetindex
                                       , const QString newstring
                                       , const QString newfile
                                       , EditorCacheList *cache
                                       , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newString = newstring;

    ptrCache = cache;
    newFile = newfile;

    oldFile = cache->at(index).profileload.filePath;
}

void ComboProfileSelect::undo()
{
    // @deprecated
//    {
//    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, index, oldFile, m_cache);
//    }

    EditorCache ec = ptrCache->at(index);
    ec.profileload.filePath = oldFile;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Profile change to %1").arg(newString) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

void ComboProfileSelect::redo()
{
    // @deprecated
//    {
//    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, index, newFile, m_cache);
//    }

    EditorCache ec = ptrCache->at(index);
    ec.profileload.filePath = newFile;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Profile change from %1").arg(m_oldstring) \
            + QString(" ^(%1,%2)").arg(index).arg(UiCommandMap::UNDOREDO_EDIT));
}

/**
 * @fn ComboProfileSelect::id
 * @brief Set id of back / forward command
 * @return Id (constant) of this back / forward command
 */
int ComboProfileSelect::id() const
{
//    ProcessXmlListGenerator pxg;
//    return pxg.getId(PR_FILEPATH);
    return UiCommandMap::PLOAD_FILEPATH;
}

bool ComboProfileSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboProfileSelect *com = static_cast<const ComboProfileSelect*>(other);
    newString = com->newString;
    newFile = com->newFile;
    return true;
}
