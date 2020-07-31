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

    EditorCache ec = ptrCache->at(targetIndex);

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:
        // Delete
        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Add the local variable to row %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;

    case UiCommandMap::LV_EDIT:

        ec.local.variantData.replace(treeIndex, oldVar);
        setText(QObject::tr("Edit the local variable '%1' on row %2").arg(newVar.variant).arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));

        break;

    case UiCommandMap::LV_INSERT:

        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Insert the local variable '%1' on row %2").arg(newVar.variant).arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;

    case UiCommandMap::LV_DELETE:

        ec.local.variantData.insert(treeIndex, oldVar);
        setText(QObject::tr("Delete the local variable of row %1").arg(treeIndex) \
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

    EditorCache ec = ptrCache->at(targetIndex);

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:

        ec.local.variantData.append(newVar);
        setText(QObject::tr("Add the local variable to row %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_ADD)));

        break;
    case UiCommandMap::LV_EDIT:

        ec.local.variantData.replace(treeIndex, newVar);
        setText(QObject::tr("Edit the local variable '%1' on row %2").arg(newVar.variant).arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));

        break;
    case UiCommandMap::LV_INSERT:

        ec.local.variantData.insert(treeIndex, newVar);
        setText(QObject::tr("Insert the local variable '%1' on row %2").arg(newVar.variant).arg(treeIndex) \
                + QString(" ^(%1,%2,%3)").arg(targetIndex).arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_INSERT)));

        break;
    case UiCommandMap::LV_DELETE:

        ec.local.variantData.removeAt(treeIndex);
        setText(QObject::tr("Delete the local variable of row %1").arg(treeIndex) \
                + QString(" ^(%1,%2)").arg(treeIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;
    default:
        break;
    }

    ptrCache->replace(targetIndex, ec);

}

int EditLocalVariantTree::id() const
{
    return tableOperation;
}

bool EditLocalVariantTree::mergeWith(const QUndoCommand *other)
{
    Q_UNUSED(other)
    return false;
}
