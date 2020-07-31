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
        setText(QObject::tr("Move local variant '%1': Up").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move local variant '%1': Down").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap local variant '%1' and '%2'").arg(arg).arg(ptrCache->at(index).local.variantData.at(source).variant) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::LV_SWAP)));
        break;
    }
}
