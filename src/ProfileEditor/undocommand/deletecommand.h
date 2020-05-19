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

#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <editorcachelist.h>

class DeleteCommand : public QUndoCommand
{
public:
//    DeleteCommand(const int &targetindex
//                  , QList<QStringList> *deleted
//                  , QList<QList<QStringList> *> *cache
//                  , QUndoCommand *parent = nullptr);

    DeleteCommand(const int &targetindex
                  , const EditorCache &remove
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;
//    QList<QStringList> *m_deleted;
//    QList<QStringList> m_delcopy;
//    QList<QList<QStringList> *> *m_cache;

    EditorCache removeCache;
    EditorCacheList *ptrCache;
};

#endif // DELETECOMMAND_H
