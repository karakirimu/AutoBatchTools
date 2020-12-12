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

#ifndef EXECTABLEOPERATION_H
#define EXECTABLEOPERATION_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditExecTable : public QUndoCommand
{
public:
    EditExecTable(const int &targetindex
                       ,const int &tableindex
                       ,const QString newstr
                       ,const int operation
                       ,EditorCacheList *cache
                       ,QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    int operation() const;

    QString newStr;

private:
    int index;
    int tableIndex;
    QString oldStr;
    int tableOperation;

    EditorCacheList *ptrCache;
};

#endif // EXECTABLEOPERATION_H
