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

#ifndef SWAPEXECTABLE_H
#define SWAPEXECTABLE_H

#include <QUndoCommand>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <editorcachelist.h>
#include <uicommandmap.h>

class SwapExecTable : public QUndoCommand
{
public:
//    SwapExecTable(const int &targetindex
//                  , const int &tablebefore
//                  , const int &tableafter
//                  , QList<QList<QStringList> *> *cache
//                  , QUndoCommand *parent = nullptr);

    SwapExecTable(const int &targetindex
                  , const int &tablebefore
                  , const int &tableafter
                  , EditorCacheList *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    int id() const override;

private:
    void replaceValue(int source, int dest);
    void setTextByCondition(int source, int dest);

    int index;
    int indexBefore;
    int indexAfter;
//    QString m_objname;
//    QList<QList<QStringList> *> *m_cache;

//    ProcessXmlListGenerator pxlg;

    int SKIP;

    EditorCacheList *ptrCache;

};

#endif // SWAPEXECTABLE_H
