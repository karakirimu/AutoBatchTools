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

#include "editlocalvartable.h"

//EditLocalVarTable::EditLocalVarTable(const int &targetindex
//                                     , const int &tableindex
//                                     , QStringList newstrlist
//                                     , const int operation
//                                     , QList<QList<QStringList> *> *cache
//                                     , QUndoCommand *parent)
//    :QUndoCommand(parent)
//{
//    index = targetindex;
//    tableIndex = tableindex;

//    m_newvar = newstrlist;
//    m_oldvar.clear();

//    tableOperation = operation;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(L_VAR_COUNT, m_cache->at(index));

//    if(tableOperation == ProcessXmlListGenerator::TABLE_EDIT
//            || tableOperation == ProcessXmlListGenerator::TABLE_DELETE){
//        m_oldvar.append(m_cache->at(index)->at(tableIndex + SKIP).at(1));
//        m_oldvar.append(m_cache->at(index)->at(tableIndex + SKIP).at(3));
//    }
//}

EditLocalVarTable::EditLocalVarTable(const int &targetindex
                                     , const int &tableindex
                                     , const QString newVariant
                                     , const QString newValue
                                     , const int operation
                                     , EditorCacheList *cache
                                     , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    tableIndex = tableindex;

    newVar = VariantPair(newVariant, newValue);
    oldVar.clear();

    tableOperation = operation;
    ptrCache = cache;

    if(tableOperation == ProcessXmlListGenerator::TABLE_EDIT
            || tableOperation == ProcessXmlListGenerator::TABLE_DELETE){
        oldVar = cache->at(index).local.variantData.at(tableIndex);
    }
}

void EditLocalVarTable::undo()
{
    qDebug() << "VariantTable: Undo: tableindex : " << tableIndex;

    // @deprecated
//    {
//    QStringList alist;
//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        //delete
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Add local at %1").arg(tableIndex) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));

//        break;

//    case ProcessXmlListGenerator::TABLE_EDIT:
//        pxlg.replaceElementList(tableIndex, index, m_oldvar, SKIP, m_cache);

//        setText(QObject::tr("Edit local at %1").arg(tableIndex) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_EDIT));
//        break;

//    case ProcessXmlListGenerator::TABLE_INSERT:
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Insert local at %1 \'%2\'").arg(tableIndex).arg(m_newvar.at(0)) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));
//        break;

//    case ProcessXmlListGenerator::TABLE_DELETE:
//        alist = ProcessXmlListGenerator::createVariantElement(m_oldvar);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Delete local at %1").arg(tableIndex) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_INS));
//        break;

//    default:
//        break;
//    }
//    }


    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add local at %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));

        break;

    case ProcessXmlListGenerator::TABLE_EDIT:

        list.replace(tableIndex, oldVar);
        setText(QObject::tr("Edit local at %1").arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_EDIT));

        break;

    case ProcessXmlListGenerator::TABLE_INSERT:
        // Delete
        list.removeAt(tableIndex);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(tableIndex).arg(newVar.variant) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));
        break;

    case ProcessXmlListGenerator::TABLE_DELETE:
        // Insert
        list.insert(tableIndex, oldVar);
        setText(QObject::tr("Delete local at %1").arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_INS));
        break;

    default:
        break;
    }

    ec.local.variantData = list;
    ptrCache->replace(index, ec);

}

void EditLocalVarTable::redo()
{
    qDebug() << "VariantTable: Redo: tableindex : " << tableIndex;

    // @deprecated
//    {
//    QStringList alist;
//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TABLE_ADD:
//        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Add local %1").arg(tableIndex) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_ADD));

//        break;
//    case ProcessXmlListGenerator::TABLE_EDIT:
//        pxlg.replaceElementList(tableIndex, index, m_newvar, SKIP, m_cache);

//        setText(QObject::tr("Edit local at %1").arg(tableIndex) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_EDIT));
//        break;
//    case ProcessXmlListGenerator::TABLE_INSERT:

//        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
//        m_cache->at(index)->insert(tableIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Insert local at %1 \'%2\'").arg(tableIndex).arg(m_newvar.at(0)) \
//                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_INS));
//        break;
//    case ProcessXmlListGenerator::TABLE_DELETE:
//        //add
//        m_cache->at(index)->removeAt(tableIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Delete local at %1").arg(tableIndex) \
//                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));
//        break;
//    default:
//        break;
//    }
//    }

    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:

        list.append(newVar);
        setText(QObject::tr("Add local %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_ADD));

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:

        list.replace(tableIndex, newVar);
        setText(QObject::tr("Edit local at %1").arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_EDIT));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:

        list.insert(tableIndex, newVar);
        setText(QObject::tr("Insert local at %1 \'%2\'").arg(tableIndex).arg(newVar.variant) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UNDOREDO_LV_INS));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:

        list.removeAt(tableIndex);
        setText(QObject::tr("Delete local at %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UNDOREDO_LV_DEL));
        break;
    default:
        break;
    }

    ec.local.variantData = list;
    ptrCache->replace(index, ec);

}

int EditLocalVarTable::id() const
{
    ProcessXmlListGenerator pxg;

    switch (tableOperation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        return pxg.getId(L_ADD_TABLE);

    case ProcessXmlListGenerator::TABLE_EDIT:
        return pxg.getId(L_EDIT_TABLE);

    case ProcessXmlListGenerator::TABLE_INSERT:
        return pxg.getId(L_INSERT_TABLE);

    case ProcessXmlListGenerator::TABLE_DELETE:
        return pxg.getId(L_DELETE_TABLE);

    }

    return pxg.getId(L_VAR_COUNT);
}

//bool EditLocalVarTable::mergeWith(const QUndoCommand *other)
//{
//    Q_UNUSED(other)
////    if (other->id() != id()) return false;
////    const EditLocalVarTable *com = static_cast<const EditLocalVarTable *>(other);
////    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
////        m_newvar = com->m_newvar;
////    }else{
////        return false;
////    }
////    return true;
//    return false;
//}

//int EditLocalVarTable::operation() const
//{
//    return m_operation;
//}

//void EditLocalVarTable::updateCounter(bool ascend)
//{
//    QStringList alist;
//    //index size update
//    alist = m_cache->at(index)->at(SKIP - 1);
//    int rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(index))).toInt();
//    if(ascend){
//        alist.replace(1, QString::number(rcount + 1));
//    }else{
//        alist.replace(1, QString::number(rcount - 1));
//    }
//    m_cache->at(index)->replace(SKIP - 1, alist);
//}
