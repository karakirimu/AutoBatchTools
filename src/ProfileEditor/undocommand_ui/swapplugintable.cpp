/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "swapplugintable.h"

SwapPluginTable::SwapPluginTable(const int &targetindex
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

void SwapPluginTable::undo()
{
    replaceValue(indexAfter, indexBefore);
    setTextByCondition(indexAfter, indexBefore);
}

void SwapPluginTable::redo()
{
    replaceValue(indexBefore, indexAfter);
    setTextByCondition(indexBefore, indexAfter);
}

int SwapPluginTable::id() const
{
    return UiCommandMap::PL_SWAP_TABLE;
}

void SwapPluginTable::replaceValue(int source, int dest)
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    list.swapItemsAt(source, dest);
    ec.exec.command = list;
    ptrCache->replace(index, ec);
}

void SwapPluginTable::setTextByCondition(int source, int dest)
{
    int condition = dest - source;
    QString arg = ptrCache->at(index).plugin.command.at(dest);

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Move plugin argument '%1': Up").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Move plugin argument '%1': Down").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap plugin argument '%1' and '%2'").arg(arg).arg(ptrCache->at(index).plugin.command.at(source)) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    }
}
