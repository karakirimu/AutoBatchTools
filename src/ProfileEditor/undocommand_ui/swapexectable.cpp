/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "swapexectable.h"

SwapExecTable::SwapExecTable(const int &targetindex
                             , const int &tablebefore
                             , const int &tableafter
                             , EditorCacheList *cache
                             , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    indexBefore = tablebefore;
    indexAfter = tableafter;
    ptrCache = cache;
}

void SwapExecTable::undo()
{
    replaceValue(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);
}

void SwapExecTable::redo()
{
    replaceValue(indexBefore, indexAfter);
    setTextByCondition(indexBefore, indexAfter);
}

int SwapExecTable::id() const
{
    return UiCommandMap::E_SWAP_TABLE;
}

void SwapExecTable::replaceValue(int source, int dest)
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    list.swapItemsAt(source, dest);
    ec.exec.command = list;
    ptrCache->replace(index, ec);
}

void SwapExecTable::setTextByCondition(int source, int dest)
{
    int condition = dest - source;
    QString arg = ptrCache->at(index).exec.command.at(dest);

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Move execution argument '%1': Up").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::E_SWAP_TABLE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move execution argument '%1': Down").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::E_SWAP_TABLE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap execution argument '%1' and '%2'").arg(arg).arg(ptrCache->at(index).exec.command.at(source)) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::E_SWAP_TABLE)));
        break;
    }
}
