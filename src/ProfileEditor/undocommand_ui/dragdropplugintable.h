/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef DRAGDROPPLUGINTABLE_H
#define DRAGDROPPLUGINTABLE_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class DragDropPluginTable : public QUndoCommand
{
public:
    DragDropPluginTable(const int &targetindex
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

#endif // DRAGDROPPLUGINTABLE_H
