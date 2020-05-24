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
        setText(QObject::tr("Up plugin at %1").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down plugin at %1").arg(arg) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap plugin at %1 %2 to %3").arg(arg).arg(source).arg(dest) \
                + QString(" ^(%1,%2,%3)").arg(source).arg(dest).arg(UiCommandMap::Id(UiCommandMap::PL_SWAP_TABLE)));
        break;
    }
}
