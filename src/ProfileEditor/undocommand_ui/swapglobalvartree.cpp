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
        setText(QObject::tr("Move global variant '%1': Up").arg(variantFrom(source)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move global variant '%1': Down").arg(variantFrom(source)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap global variant '%1' and '%2'").arg(variantFrom(source)).arg(variantFrom(dest)) \
                + QString(" ^(%1,%2,%3)").arg(indexBefore).arg(indexAfter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    }
}
