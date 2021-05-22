/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editlocalvartable.h"

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

    if(tableOperation == UiCommandMap::LV_EDIT
            || tableOperation == UiCommandMap::LV_DELETE){
        oldVar = cache->at(index).local.variantData.at(tableIndex);
    }
}

void EditLocalVarTable::undo()
{
    qDebug() << "VariantTable: Undo: tableindex : " << tableIndex;

    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:
        // Delete
        list.removeAt(tableIndex);
        setText(QObject::tr("Add the local variable to row %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));

        break;

    case UiCommandMap::LV_EDIT:

        list.replace(tableIndex, oldVar);
        setText(QObject::tr("Edit the local variable '%1' on row %2").arg(newVar.variant).arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));

        break;

    case UiCommandMap::LV_INSERT:
        // Delete
        list.removeAt(tableIndex);

        setText(QObject::tr("Insert the local variable '%1' on row %2").arg(newVar.variant).arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));
        break;

    case UiCommandMap::LV_DELETE:
        // Insert
        list.insert(tableIndex, oldVar);
        setText(QObject::tr("Delete the local variable of row %1").arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_INSERT)));
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

    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;

    switch (tableOperation) {
    case UiCommandMap::LV_ADD:

        list.append(newVar);
        setText(QObject::tr("Add the local variable to row %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_ADD)));

        break;
    case UiCommandMap::LV_EDIT:

        list.replace(tableIndex, newVar);
        setText(QObject::tr("Edit the local variable '%1' on row %2").arg(newVar.variant).arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_EDIT)));
        break;
    case UiCommandMap::LV_INSERT:

        list.insert(tableIndex, newVar);
        setText(QObject::tr("Insert the local variable '%1' on row %2").arg(newVar.variant).arg(tableIndex) \
                + QString(" ^(%1,%2,%3)").arg(index).arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_INSERT)));
        break;
    case UiCommandMap::LV_DELETE:

        list.removeAt(tableIndex);
        setText(QObject::tr("Delete the local variable of row %1").arg(tableIndex) \
                + QString(" ^(%1,%2)").arg(tableIndex).arg(UiCommandMap::Id(UiCommandMap::LV_DELETE)));
        break;
    default:
        break;
    }

    ec.local.variantData = list;
    ptrCache->replace(index, ec);

}

int EditLocalVarTable::id() const
{
    return tableOperation;
}
