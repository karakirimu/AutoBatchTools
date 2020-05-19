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

#include "editsearchoutput.h"

//EditSearchOutput::EditSearchOutput(const int &targetindex
//                                   , int newvalue
//                                   , QList<QList<QStringList> *> *cache
//                                   , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    newValue = newvalue;
//    m_cache = cache;

//    oldValue = static_cast<QString>(pxlg.fetch(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, m_cache->at(index))).toInt();

//}

EditSearchOutput::EditSearchOutput(const int &targetindex
                                   , const int &newvalue
                                   , EditorCacheList *cache
                                   , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newValue = newvalue;
    ptrCache = cache;

    oldValue = cache->at(index).filesearch.outputOption;
}

void EditSearchOutput::undo()
{
    // @deprecated
//    {
//    if(m_cache->isEmpty()) return;
//    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, index, QString::number(oldValue), m_cache);
//    }

    replaceValue(oldValue);

    setText(QObject::tr("Output Select") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void EditSearchOutput::redo()
{
    // @deprecated
//    {
//    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, index, QString::number(newValue), m_cache);
//    }

    replaceValue(newValue);

    setText(QObject::tr("Output Select") \
            + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int EditSearchOutput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(ATTR_RADIOBUTTONPOS);
}

bool EditSearchOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchOutput *com = static_cast<const EditSearchOutput *>(other);
    newValue = com->newValue;
    return true;
}

void EditSearchOutput::replaceValue(int value)
{
    EditorCache ec = ptrCache->at(index);
    ec.filesearch.outputOption = value;
    ptrCache->replace(index, ec);
}
