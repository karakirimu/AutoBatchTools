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

#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include <QUndoCommand>
#include <CommandMap.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <editorcachelist.h>

class InsertCommand : public QUndoCommand
{
public:
//    InsertCommand(const int &targetindex
//                  , QList<QStringList> *inserted
//                  , QList<QList<QStringList> *> *cache
//                  , QUndoCommand *parent = nullptr);

    InsertCommand(const int &targetindex
                  , const EditorCache &insert
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int index;
//    QList<QStringList> *m_inserted;
//    QList<QStringList> m_inscopy;
//    QList<QList<QStringList> *> *m_cache;

    EditorCache insertCache;
    EditorCacheList *ptrCache;
};

#endif // INSERTCOMMAND_H
