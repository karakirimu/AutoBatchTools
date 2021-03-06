/*
 * Copyright 2016-2021 karakirimu
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

#include "editglobalvartree.h"

EditGlobalVarTree::EditGlobalVarTree(const int &treeindex
                                     , const QStringList &variants
                                     , const int &operation
                                     , QUndoCommand *parent)
    : QUndoCommand (parent)
{
    m_treeindex = treeindex;
    m_newvar = variants;
    m_oldvar.clear();

    m_operation = operation;

    if(m_operation == UiCommandMap::GV_EDIT_TREE
            || m_operation == UiCommandMap::GV_DEL_TREE){
        QList<QStringList> old;
        if(sxml.readItem(m_treeindex, &old)){
            m_oldvar.append(old.at(0).at(1));
            m_oldvar.append(old.at(1).at(1));
        }
    }
}

void EditGlobalVarTree::undo()
{
    QList<QStringList> xmlformat;

    qDebug() << "VariantTable: Undo: tableindex : " << m_treeindex;
    switch (m_operation) {
    case UiCommandMap::GV_ADD_TREE:
        //delete
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Add the global variable to row %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_DEL_TREE)));

        break;

    case UiCommandMap::GV_EDIT_TREE:
        sxml.createVarElement(&xmlformat, &m_oldvar);
        sxml.editItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Edit the global variable '%1' on row %2").arg(m_newvar.at(0)).arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_EDIT_TREE)));
        break;

    case UiCommandMap::GV_INS_TREE:
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Insert the global variable '%1' on row %2").arg(m_newvar.at(0)).arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_DEL_TREE)));
        break;

    case UiCommandMap::GV_DEL_TREE:
        sxml.createVarElement(&xmlformat, &m_oldvar);
        sxml.insertItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Delete the global variable of row %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_INS_TREE)));
        break;

    default:
        break;
    }

}

void EditGlobalVarTree::redo()
{
    QList<QStringList> xmlformat;
    qDebug() << "VariantTable: Redo: tableindex : " << m_treeindex;


    switch (m_operation) {
    case UiCommandMap::GV_ADD_TREE:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.addItem(&xmlformat);

        setText(QObject::tr("Add the global variable to row %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_ADD_TREE)));
        break;

    case UiCommandMap::GV_EDIT_TREE:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.editItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Edit the global variable '%1' on row %2").arg(m_newvar.at(0)).arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_EDIT_TREE)));
        break;

    case UiCommandMap::GV_INS_TREE:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.insertItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Insert the global variable '%1' on row %2").arg(m_newvar.at(0)).arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_INS_TREE)));
        break;

    case UiCommandMap::GV_DEL_TREE:
        //add
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Delete the global variable of row %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UiCommandMap::Id(UiCommandMap::GV_DEL_TREE)));
        break;

    default:
        break;
    }
}

int EditGlobalVarTree::id() const
{
    return m_operation;
}

bool EditGlobalVarTree::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    if(m_operation == UiCommandMap::GV_EDIT_TREE){
        const EditGlobalVarTree *com = static_cast<const EditGlobalVarTree*>(other);
        m_newvar = com->m_newvar;
    }
    return true;
}
