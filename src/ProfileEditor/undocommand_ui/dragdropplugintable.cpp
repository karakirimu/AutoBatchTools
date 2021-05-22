/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "dragdropplugintable.h"

DragDropPluginTable::DragDropPluginTable(const int &targetindex
                                         , const QList<int> &tablebefore
                                         , const int &tableafter
                                         , EditorCacheList *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    index = targetindex;
    indexBefore = tablebefore;
    indexAfter = tableafter;

    ptrCache = cache;

    //sort list assend order
    std::sort(indexBefore.begin(), indexBefore.end());

    for(int i = 0; i < indexBefore.count(); i++){
        oldStr.insert(indexBefore.at(i)
                        , cache->at(index).plugin.command.at(indexBefore.at(i)));
    }
}

void DragDropPluginTable::undo()
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    //delete insert

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int before = 0;
    QString beforedata;
    int sourcecount = indexBefore.count();

    for(int i = 0; i < sourcecount; i++){
        before = indexBefore.last();

        if(before > indexAfter){

            //down to up operation
            if(!lastelement){
                lastelement = true;
                deleterow = indexAfter;
            }

            beforedata = oldStr.value(indexBefore.at(i));

        }else{

            //up to down operation
            if(!firstelement){
                firstelement = true;
                deleterow = indexAfter - 1;
            }

            before = indexBefore.first();

            beforedata = oldStr.value(indexBefore.at(sourcecount - 1 - i));
        }

        list.removeAt(deleterow);
        list.insert(before, beforedata);
    }

    ec.plugin.command = list;
    ptrCache->replace(index, ec);

    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(index).arg(deleterow) \
                                         .arg(indexBefore.count()).arg(before) \
                                         .arg(UiCommandMap::Id(UiCommandMap::PL_MOVE_TABLE));
    setText(QObject::tr("Move plugin argument to row '%1'").arg(QString::number(indexAfter)) + sendcode);
}

void DragDropPluginTable::redo()
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.plugin.command;
    QString sendcode;

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int updown = 0;
    int before = 0;
    QString beforedata;
    int sourcecount = indexBefore.count();

    int deductnum = 1;

    //multiple element move
    for(int i = 0; i < sourcecount; i++){
        before = indexBefore.at(i);

        if(before > indexAfter){

            if(!lastelement){
                lastelement = true;
                deleterow = indexBefore.last();
            }

            beforedata = oldStr.value(indexBefore.at(sourcecount - deductnum));
            deductnum++;
            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = indexBefore.first();
            }

            beforedata = oldStr.value(before);
            updown = -1;

        }

        list.removeAt(deleterow);
        list.insert(indexAfter + updown, beforedata);
    }

    ec.plugin.command = list;
    ptrCache->replace(index, ec);

    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(index).arg(deleterow) \
                                         .arg(indexBefore.count()).arg(indexAfter) \
                                         .arg(UiCommandMap::Id(UiCommandMap::PL_MOVE_TABLE));

    setText(QObject::tr("Move plugin argument to row '%1'").arg(QString::number(indexAfter)) + sendcode);
}

//void DragDropPluginTable::updateIndex(int count)
//{

//}
