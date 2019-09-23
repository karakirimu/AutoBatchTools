/*
 * Copyright 2016-2019 karakirimu
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

ComboProfileSelect::ComboProfileSelect(const int &targetindex
                                       , const QString newstring
                                       , const QString newfile
                                       , QList<QList<QStringList> *> *cache
                                       , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;
    m_newfile = newfile;

    m_oldfile = pxlg.fetch(PR_FILEPATH, ATTR_NONE, m_cache->at(m_targetindex));

}

void ComboProfileSelect::undo()
{
    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, m_targetindex, m_oldfile, m_cache);

    setText(QObject::tr("Profile change to %1 ").arg(m_newstring) \
            + QString("^(%1)").arg(m_targetindex));
}

void ComboProfileSelect::redo()
{
    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, m_targetindex, m_newfile, m_cache);

    setText(QObject::tr("Profile change from %1 ").arg(m_oldstring) \
            + QString("^(%1)").arg(m_targetindex));
}

/**
 * @fn ComboProfileSelect::id
 * @brief Set id of back / forward command
 * @return Id (constant) of this back / forward command
 */
int ComboProfileSelect::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(PR_FILEPATH);
}

bool ComboProfileSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboProfileSelect *com = static_cast<const ComboProfileSelect*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
