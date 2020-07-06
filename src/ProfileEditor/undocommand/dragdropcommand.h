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

#ifndef DRAGDROPCOMMAND_H
#define DRAGDROPCOMMAND_H

#include <QUndoCommand>
#include <commandmap.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <editorcachelist.h>

class DragDropCommand : public QUndoCommand
{
public:
//    DragDropCommand(const QList<int> &beforeindex
//                    , const int &afterindex
//                    , QList<QList<QStringList> *> *cache
//                    , QUndoCommand *parent = nullptr);

    DragDropCommand(const QList<int> &beforeindex
                    , const int &afterindex
                    , EditorCacheList *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<int> beforeIndex;
    int afterIndex;

//    QList<QList<QStringList> *> *m_cache;
    EditorCacheList *ptrCache;
};

#endif // DRAGDROPCOMMAND_H
