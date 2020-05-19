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

#ifndef EDITGLOBALVARTREE_H
#define EDITGLOBALVARTREE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @brief The EditGlobalVarTree class
 * newstrlist structure
 * 0       :1       :
 * variant :value   :
 */

class EditGlobalVarTree : public QUndoCommand
{
public:
    EditGlobalVarTree(const int &treeindex
                      , const QStringList &variants
                      , const int &operation
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QStringList m_newvar;

private:
//    void updateCounter(bool ascend);
//    void setStrBuilderFormat(QList<QStringList> *internal, QStringList *var);

    int m_treeindex;
    int m_tableindex;
    QStringList m_oldvar;
    int m_operation;
//    QList<QList<QStringList> *> *m_cache;

    StringXmlBuilder sxml;

//    int SKIP;

    EditorCacheList *ptrCache;
};

#endif // EDITGLOBALVARTREE_H
