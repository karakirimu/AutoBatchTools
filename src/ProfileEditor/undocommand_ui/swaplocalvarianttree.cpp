/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "swaplocalvarianttree.h"

SwapLocalVariantTree::SwapLocalVariantTree(const int &localindex
                                           , const int &before
                                           , const int &after
                                           , EditorCacheList *cache
                                           , QUndoCommand *parent)
    : QUndoCommand (parent)
{
    index = localindex;
    indexBefore = before;
    indexAfter = after;
    ptrCache = cache;
}

void SwapLocalVariantTree::undo()
{
    replaceValue(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);
}

void SwapLocalVariantTree::redo()
{
    replaceValue(indexBefore, indexAfter);
    setTextByCondition(indexBefore, indexAfter);
}

int SwapLocalVariantTree::id() const
{
    return UiCommandMap::LV_SWAP;
}

void SwapLocalVariantTree::replaceValue(int source, int dest)
{
    EditorCache ec = ptrCache->at(index);
    QList<VariantPair> list = ec.local.variantData;
    list.swapItemsAt(source, dest);
    ec.local.variantData = list;
    ptrCache->replace(index, ec);
}

void SwapLocalVariantTree::setTextByCondition(int source, int dest)
{
    int condition = dest - source;
    QString arg = ptrCache->at(index).local.variantData.at(dest).variant;

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Move local variable '%1': Up").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move local variable '%1': Down").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap local variable '%1' and '%2'").arg(arg).arg(ptrCache->at(index).local.variantData.at(source).variant) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    }
}
