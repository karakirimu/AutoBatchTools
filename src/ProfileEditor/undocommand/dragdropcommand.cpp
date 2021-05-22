/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "dragdropcommand.h"

DragDropCommand::DragDropCommand(const QList<int> &beforeindex
                                 , const int &afterindex
                                 , EditorCacheList *cache
                                 , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    beforeIndex = beforeindex;
    afterIndex = afterindex;
    ptrCache = cache;
}

void DragDropCommand::undo()
{
    QString sendcode;

    //delete insert
    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int before = 0;
    int deductnum = 1;

    EditorCache beforedata;
    int sourcecount = beforeIndex.count();

    for(int i = 0; i < sourcecount; i++){
        before = beforeIndex.last();

        if(before > afterIndex){

            //down to up operation
            if(!lastelement){
                lastelement = true;
                deleterow = afterIndex;
            }

            beforedata = ptrCache->at(deleterow);

        }else{

            //up to down operation
            if(!firstelement){
                firstelement = true;
                deleterow = afterIndex - 1;
            }

            before = beforeIndex.first();

            beforedata = ptrCache->at(deleterow);
            deductnum++;

        }

        ptrCache->removeAt(deleterow);
        ptrCache->insert(before, beforedata);
    }

    //  "process" first before(source) index,
    //  "process" before(source) contiguous count (incremental),
    //  "process" after(target) index,
    //  sendcode
    sendcode = QString(" ^(%1,%2,%3,%4)").arg(deleterow) \
                                         .arg(beforeIndex.count()).arg(before) \
                                         .arg(CommandMap::UNDOREDO_MOVE);
    setText(QObject::tr("Move item at No. %1").arg(QString::number(afterIndex)) + sendcode);
}

void DragDropCommand::redo()
{
    QString sendcode;

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int updown = 0;
    int before = 0;

    EditorCache beforedata;
    int sourcecount = beforeIndex.count();

    //multiple element move
    for(int i = 0; i < sourcecount; i++){
        before = beforeIndex.at(i);

        if(before > afterIndex){

            if(!lastelement){
                lastelement = true;
                deleterow = beforeIndex.last();
            }

            beforedata = ptrCache->at(deleterow);
            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = beforeIndex.first();
            }

            beforedata = ptrCache->at(deleterow);
            updown = -1;

        }

        ptrCache->removeAt(deleterow);
        ptrCache->insert(afterIndex + updown, beforedata);
    }

    //  "process" first before(source) index,
    //  "process" before(source) contiguous count (incremental),
    //  "process" after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4)").arg(deleterow) \
                                         .arg(beforeIndex.count()).arg(afterIndex) \
                                         .arg(CommandMap::UNDOREDO_MOVE);

    setText(QObject::tr("Move item at No. %1").arg(QString::number(afterIndex)) + sendcode);

}
