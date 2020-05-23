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

#ifndef STRINGPROJECTNAME_H
#define STRINGPROJECTNAME_H

#include <QUndoCommand>
#include <editorcachelist.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>

class StringProjectName : public QUndoCommand
{
public:
//    StringProjectName(const int &targetindex
//                      , QString newstring
//                      , QList<QList<QStringList> *> *cache
//                      , QUndoCommand *parent = nullptr);

    StringProjectName(const int &targetindex
                      , const QString &newstring
                      , EditorCacheList *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newStr;
private:
    int index;
    QString oldStr;
//    QList<QList<QStringList> *> *m_cache;
//    ProcessXmlListGenerator pxlg;

    EditorCacheList *ptrCache;
};

#endif // STRINGPROJECTNAME_H
