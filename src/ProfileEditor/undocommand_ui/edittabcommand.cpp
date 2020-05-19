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

#include "edittabcommand.h"

//EditTabCommand::EditTabCommand(const int &targetindex
//                               , const int &newid
//                               , QList<QList<QStringList>* > *cache
//                               , QUndoCommand *parent)
//    : QUndoCommand(parent)
//{
//    index = targetindex;
//    newIndex = newid;
//    m_cache = cache;

//    if(m_cache->at(index)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
//        //temp index
//        oldIndex = static_cast<QString>(pxlg.fetch(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_cache->at(index))).toInt();
//    }else{
//        //no istack
//        oldIndex = pxlg.getType(pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(index)));
//    }

////    m_oldauto = pxlg.fetch(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));

////    selectIndex(m_newid);

//}

EditTabCommand::EditTabCommand(const int &targetindex
                               , const int &newindex
                               , EditorCacheList *cache
                               , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    newIndex = newindex;
    ptrCache = cache;
    oldType = cache->at(index).type;
    oldIndex = cache->at(index).functionSelect;
}

void EditTabCommand::undo()
{    
//    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldauto, m_cache);

    // @deprecated
//    {
//    if(m_cache->at(index)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
//        //temp index
//        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, index, QString::number(oldIndex), m_cache);
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    FunctionType ft;
    ec.type = oldType;
    ec.functionSelect = oldIndex;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Type change") + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

void EditTabCommand::redo()
{
//    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_newauto, m_cache);

    // @deprecated
//    {
//    if(m_cache->at(index)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
//        //temp index
//        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, index, QString::number(newIndex), m_cache);
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    FunctionType ft;
    if(ec.type != ft.getString(ft.TYPE::ALLINCLUDE)){
        ec.type = ft.getString(ft.TYPE::ALLINCLUDE);
    }
    ec.functionSelect = newIndex;
    ptrCache->replace(index, ec);

    setText(QObject::tr("Type change") + QString(" ^(%1,%2)").arg(index).arg(UNDOREDO_EDIT));
}

int EditTabCommand::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(TE_STACKEDWIDGET_POSITION);
}

//void EditTabCommand::selectIndex(int index)
//{
//    switch (index) {
//    case ProcessXmlListGenerator::NORMAL:
//        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::NORMAL);
//        break;
//    case ProcessXmlListGenerator::SEARCH:
//        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::SEARCH);
//        break;
//    case ProcessXmlListGenerator::EXTRAFUNC:
//        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::EXTRAFUNC);
//        break;
//    case ProcessXmlListGenerator::OTHER:
//        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::OTHER);
//        break;
//    default:
//        break;
//    }
//}
