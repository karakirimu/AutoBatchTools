/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef DRAGDROPEXECTABLE_H
#define DRAGDROPEXECTABLE_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class DragDropExecTable : public QUndoCommand
{
public:
    DragDropExecTable(const int &targetindex
                      , const QList<int> &tablebefore
                      , const int &tableafter
                      , EditorCacheList *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;
    QList<int> indexBefore;
    int indexAfter;
    QHash<int, QString> oldStr;

    EditorCacheList *ptrCache;

};

#endif // DRAGDROPEXECTABLE_H
