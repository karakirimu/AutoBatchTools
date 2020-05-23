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

#include "editlocalvarianttree.h"

//EditLocalVariantTree::EditLocalVariantTree(const int &targetindex
//                                           , const int &treeindex
//                                           , QStringList variants
//                                           , const int operation
//                                           , QList<QList<QStringList> *> *cache
//                                           , QUndoCommand *parent)
//    :QUndoCommand (parent)
//{
//    targetIndex = targetindex;
//    treeIndex = treeindex;

//    m_newvar = variants;
//    m_oldvar.clear();

//    tableOperation = operation;
//    m_cache = cache;

//    SKIP = pxlg.fetchCmdFirstPos(L_VAR_COUNT, m_cache->at(targetIndex));

//    if(tableOperation == ProcessXmlListGenerator::TREE_EDIT
//            || tableOperation == ProcessXmlListGenerator::TREE_DELETE){
//        m_oldvar.append(m_cache->at(targetIndex)->at(treeIndex + SKIP).at(1));
//        m_oldvar.append(m_cache->at(targetIndex)->at(treeIndex + SKIP).at(3));
//    }
//}

EditLocalVariantTree::EditLocalVariantTree(const int &targetindex
                                           , const int &treeindex
                                           , const QString &localvariant
                                           , const QString &localvalue
                                           , const int operation
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    targetIndex = targetindex;
    treeIndex = treeindex;

    newVar = VariantPair(localvariant, localvalue);
    oldVar.clear();

    tableOperation = operation;
    ptrCache = cache;

    if(tableOperation == UiCommandMap::LV_EDIT
            || tableOperation == UiCommandMap::LV_DELETE){
        oldVar = cache->at(targetIndex).local.variantData.at(treeIndex);
    }
}

void EditLocalVariantTree::undo()
{
    qDebug() << "[EditLocalVariantTree::undo()] treeindex : " << treeIndex;

//    QStringList alist;
//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TREE_ADD:
//        //delete
//        m_cache->at(targetIndex)->removeAt(treeIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Add local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2)").arg(treeIndex).arg(UNDOREDO_LV_DEL));

//        break;

//    case ProcessXmlListGenerator::TREE_EDIT:
//        pxlg.replaceElementList(treeIndex, targetIndex, m_oldvar, SKIP, m_cache);

//        setText(QObject::tr("Edit local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UNDOREDO_LV_EDIT));
//        break;

//    case ProcessXmlListGenerator::TREE_INSERT:
//        m_cache->at(targetIndex)->removeAt(treeIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Insert local at %1 \'%2\'").arg(treeIndex).arg(m_newvar.at(0)) \
//                + QString(" ^(%1,%2)").arg(treeIndex).arg(UNDOREDO_LV_DEL));
//        break;

//    case ProcessXmlListGenerator::TREE_DELETE:
//        alist = ProcessXmlListGenerator::createVariantElement(m_oldvar);
//        m_cache->at(targetIndex)->insert(treeIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Delete local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UNDOREDO_LV_INS));
//        break;

//    default:
//        break;
//    }

    EditorCache ec = ptrCache->at(targetIndex);

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:
        // Delete
        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Add local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;

    case UiCommandMap::LV_EDIT:

        ec.local.variantData.replace(treeIndex, oldVar);
        setText(QObject::tr("Edit local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));

        break;

    case UiCommandMap::LV_INSERT:

        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Insert local at %1 \'%2\'").arg(treeIndex).arg(newVar.variant) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;

    case UiCommandMap::LV_DELETE:

        ec.local.variantData.insert(treeIndex, oldVar);
        setText(QObject::tr("Delete local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_INSERT)));

        break;

    default:
        break;
    }

    ptrCache->replace(targetIndex, ec);

}

void EditLocalVariantTree::redo()
{
    qDebug() << "[EditLocalVariantTree::redo()] treeindex : " << treeIndex;

//    QStringList alist;
//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TREE_ADD:
//        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
//        m_cache->at(targetIndex)->insert(treeIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Add local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2)").arg(treeIndex).arg(UNDOREDO_LV_ADD));

//        break;
//    case ProcessXmlListGenerator::TREE_EDIT:
//        pxlg.replaceElementList(treeIndex, targetIndex, m_newvar, SKIP, m_cache);

//        setText(QObject::tr("Edit local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UNDOREDO_LV_EDIT));
//        break;
//    case ProcessXmlListGenerator::TREE_INSERT:

//        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
//        m_cache->at(targetIndex)->insert(treeIndex + SKIP, alist);

//        updateCounter(true);

//        setText(QObject::tr("Insert local at %1 \'%2\'").arg(treeIndex).arg(m_newvar.at(0)) \
//                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UNDOREDO_LV_INS));
//        break;
//    case ProcessXmlListGenerator::TREE_DELETE:
//        //add
//        m_cache->at(targetIndex)->removeAt(treeIndex + SKIP);

//        updateCounter(false);

//        setText(QObject::tr("Delete local at %1").arg(treeIndex) \
//                + QString(" ^(%1,%2)").arg(treeIndex).arg(UNDOREDO_LV_DEL));
//        break;
//    default:
//        break;
//    }

    EditorCache ec = ptrCache->at(targetIndex);

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:

        ec.local.variantData.append(newVar);
        setText(QObject::tr("Add local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_ADD)));

        break;
    case UiCommandMap::LV_EDIT:

        ec.local.variantData.replace(treeIndex, newVar);
        setText(QObject::tr("Edit local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));

        break;
    case UiCommandMap::LV_INSERT:

        ec.local.variantData.insert(treeIndex, newVar);
        setText(QObject::tr("Insert local at %1 \'%2\'").arg(treeIndex).arg(newVar.variant) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_INSERT)));

        break;
    case UiCommandMap::LV_DELETE:

        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Delete local at %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;
    default:
        break;
    }

    ptrCache->replace(targetIndex, ec);

}

int EditLocalVariantTree::id() const
{
//    ProcessXmlListGenerator pxg;

//    switch (tableOperation) {
//    case ProcessXmlListGenerator::TREE_ADD:
//        return pxg.getId(UNDOREDO_LV_ADD);

//    case ProcessXmlListGenerator::TREE_EDIT:
//        return pxg.getId(UNDOREDO_LV_EDIT);

//    case ProcessXmlListGenerator::TREE_INSERT:
//        return pxg.getId(UNDOREDO_LV_INS);

//    case ProcessXmlListGenerator::TREE_DELETE:
//        return pxg.getId(UNDOREDO_LV_DEL);

//    }

//    return pxg.getId(L_VAR_COUNT);
    return tableOperation;
}

bool EditLocalVariantTree::mergeWith(const QUndoCommand *other)
{
    Q_UNUSED(other)
    return false;
}

//void EditLocalVariantTree::updateCounter(bool ascend)
//{
//    QStringList alist;
//    //index size update
//    alist = m_cache->at(targetIndex)->at(SKIP - 1);
//    int rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(targetIndex))).toInt();
//    if(ascend){
//        alist.replace(1, QString::number(rcount + 1));
//    }else{
//        alist.replace(1, QString::number(rcount - 1));
//    }
//    m_cache->at(targetIndex)->replace(SKIP - 1, alist);
//}
