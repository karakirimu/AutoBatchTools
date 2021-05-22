/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "swapglobalvartree.h"

SwapGlobalVarTree::SwapGlobalVarTree(const int &before
                                     , const int &after
                                     , QUndoCommand *parent)
    :QUndoCommand (parent)
{
    indexBefore = before;
    indexAfter = after;
}

void SwapGlobalVarTree::undo()
{
    sxml.swapItem(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);
}

void SwapGlobalVarTree::redo()
{
    sxml.swapItem(indexBefore, indexAfter);
    setTextByCondition(indexAfter, indexBefore);
}

int SwapGlobalVarTree::id() const
{
    return UiCommandMap::GV_SWAP_TREE;
}

void SwapGlobalVarTree::setTextByCondition(int source, int dest)
{
    int condition = dest - source;

    auto variantFrom = [&](int index){
        QList<QStringList> newvar;
        QString vari = "";
        if(sxml.readItem(index, &newvar)){
            vari = newvar.at(0).at(1);
        }
        return vari;
    };

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Move the global variable '%1': Up").arg(variantFrom(source)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move the global variable '%1': Down").arg(variantFrom(source)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap the global variable '%1' and '%2'").arg(variantFrom(source)).arg(variantFrom(dest)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    }
}
