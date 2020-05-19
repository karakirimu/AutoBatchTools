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

    if(m_operation == ProcessXmlListGenerator::TREE_EDIT
            || m_operation == ProcessXmlListGenerator::TREE_DELETE){
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
    case ProcessXmlListGenerator::TREE_ADD:
        //delete
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Add global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_DEL));

        break;

    case ProcessXmlListGenerator::TREE_EDIT:
        sxml.createVarElement(&xmlformat, &m_oldvar);
        sxml.editItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Edit global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_EDIT));
        break;

    case ProcessXmlListGenerator::TREE_INSERT:
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Insert global at %1 \'%2\'").arg(m_treeindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_DEL));
        break;

    case ProcessXmlListGenerator::TREE_DELETE:
        sxml.createVarElement(&xmlformat, &m_oldvar);
        sxml.insertItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Delete global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_INS));
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
    case ProcessXmlListGenerator::TREE_ADD:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.addItem(&xmlformat);

        setText(QObject::tr("Add global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_ADD));
        break;

    case ProcessXmlListGenerator::TREE_EDIT:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.editItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Edit global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_EDIT));
        break;

    case ProcessXmlListGenerator::TREE_INSERT:
        sxml.createVarElement(&xmlformat, &m_newvar);
        sxml.insertItem(m_treeindex, &xmlformat);

        setText(QObject::tr("Insert global at %1 \'%2\'").arg(m_treeindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_INS));
        break;

    case ProcessXmlListGenerator::TREE_DELETE:
        //add
        sxml.deleteItem(m_treeindex);

        setText(QObject::tr("Delete global at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_GV_DEL));
        break;

    default:
        break;
    }
}

int EditGlobalVarTree::id() const
{
    ProcessXmlListGenerator pxg;

    switch (m_operation) {
    case ProcessXmlListGenerator::TREE_ADD:
        return pxg.getId(UNDOREDO_GV_ADD);

    case ProcessXmlListGenerator::TREE_EDIT:
        return pxg.getId(UNDOREDO_GV_EDIT);

    case ProcessXmlListGenerator::TREE_INSERT:
        return pxg.getId(UNDOREDO_GV_INS);

    case ProcessXmlListGenerator::TREE_DELETE:
        return pxg.getId(UNDOREDO_GV_DEL);

    }

    //todo: no suitable difinition class
    return 9000;
}

bool EditGlobalVarTree::mergeWith(const QUndoCommand *other)
{
    Q_UNUSED(other)
    return false;
}

//void EditGlobalVarTree::setStrBuilderFormat(QList<QStringList> *internal, QStringList *var)
//{
//    internal->append(QStringList() << "variant" << var->at(0));
//    internal->append(QStringList() << "value" << var->at(1));
//}
