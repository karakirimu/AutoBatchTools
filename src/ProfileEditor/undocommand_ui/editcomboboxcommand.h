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

#ifndef EDITCOMBOBOXCOMMAND_H
#define EDITCOMBOBOXCOMMAND_H

//for localvariant combobox
#include <QUndoCommand>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditComboBoxCommand : public QUndoCommand
{
public:
//    EditComboBoxCommand(const int &targetindex
//                        , const QString newstring
//                        , QList<QList<QStringList> *> *cache
//                        , QUndoCommand *parent = nullptr);

    EditComboBoxCommand(const int &targetindex
                        , const QString newstring
                        , EditorCacheList *cache
                        , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newString;
private:
    int index;
    QString oldString;
//    QList<QList<QStringList> *> *m_cache;
//    ProcessXmlListGenerator pxlg;

    EditorCacheList *ptrCache;

};

#endif // EDITCOMBOBOXCOMMAND_H
