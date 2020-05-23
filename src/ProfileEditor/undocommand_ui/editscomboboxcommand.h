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

#ifndef EDITSCOMBOBOXCOMMAND_H
#define EDITSCOMBOBOXCOMMAND_H

#include <QUndoCommand>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>
#include <editorcachelist.h>

//for search combobox
class EditScomboBoxCommand : public QUndoCommand
{
public:
//    EditScomboBoxCommand(const int &targetindex
//                         , const QString &newstring
//                         , const int &newsearchindex
//                         , QList<QList<QStringList> *> *cache
//                         , QUndoCommand *parent = nullptr);

    EditScomboBoxCommand(const int &targetindex
                         , const QString &newstring
                         , const int &newsearchindex
                         , EditorCacheList *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
private:
    void replace(QString str, int i);

    int index;
    QString oldString;
    QString newString;
    int oldIndex;
    int newIndex;

    FunctionType::TYPE select;

//    QList<QList<QStringList> *> *m_cache;
//    QHash<int, int> posinfo;
//    int m_indexpos = 0;

    EditorCacheList *ptrCache;

//    ProcessXmlListGenerator pxlg;

};

#endif // EDITSCOMBOBOXCOMMAND_H
