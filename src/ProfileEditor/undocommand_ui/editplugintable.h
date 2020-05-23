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

#ifndef EXTRATABLECOMMAND_H
#define EXTRATABLECOMMAND_H

#include <QUndoCommand>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditPluginTable : public QUndoCommand
{
public:
//    EditPluginTable(const int &targetindex
//                      ,const int &tableindex
//                      ,QString newstr
//                      ,const int operation
//                      ,QList<QList<QStringList> *> *cache
//                      ,QUndoCommand *parent = nullptr);

//    EditPluginTable(const int &targetindex
//                     , QStringList newstrlist
//                     , const int operation
//                     , QList<QList<QStringList> *> *cache
//                     , QUndoCommand *parent = nullptr);

    EditPluginTable(const int &targetindex
                      ,const int &tableindex
                      ,const QString newstr
                      ,const int operation
                      ,EditorCacheList *cache
                      ,QUndoCommand *parent = nullptr);

    EditPluginTable(const int &targetindex
                     , const QStringList newstrlist
                     , const int operation
                     , EditorCacheList *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newStr;
    QStringList newStrList;

private:
//    void updateIndex(int count);
//    void updateCounter(bool ascend);

    int index;
    int tableIndex;
    QString oldStr;
    QStringList oldStrList;
    int tableOperation;
//    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;

//    ProcessXmlListGenerator pxlg;

    //! First position of PluginCommandTable
//    int SKIP;

    EditorCacheList *ptrCache;
};

#endif // EXTRATABLECOMMAND_H
