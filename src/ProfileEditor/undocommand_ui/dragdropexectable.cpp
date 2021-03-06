/*
 * Copyright 2016-2021 karakirimu
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

#include "dragdropexectable.h"

DragDropExecTable::DragDropExecTable(const int &targetindex
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
                        , cache->at(index).exec.command.at(indexBefore.at(i)));
    }
}

void DragDropExecTable::undo()
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
    QString sendcode;

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

    ec.exec.command = list;
    ptrCache->replace(index, ec);

    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(index).arg(deleterow) \
                                         .arg(indexBefore.count()).arg(before) \
                                         .arg(UiCommandMap::Id(UiCommandMap::E_MOVE_TABLE));
    setText(QObject::tr("Move execution argument to row '%1'").arg(QString::number(indexAfter)) + sendcode);
}

void DragDropExecTable::redo()
{
    EditorCache ec = ptrCache->at(index);
    QStringList list = ec.exec.command;
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

    ec.exec.command = list;
    ptrCache->replace(index, ec);

    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(index).arg(deleterow) \
                                         .arg(indexBefore.count()).arg(indexAfter) \
                                         .arg(UiCommandMap::Id(UiCommandMap::E_MOVE_TABLE));

    setText(QObject::tr("Move execution argument to row '%1'").arg(QString::number(indexAfter)) + sendcode);
}
