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

#ifndef EDITVARIANTTREE_H
#define EDITVARIANTTREE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @brief The EditLocalVariantTree class
 * newstrlist structure
 * 0       :1       :
 * variant :value   :
 */

class EditLocalVariantTree : public QUndoCommand
{
public:
//    EditLocalVariantTree(const int &targetindex
//                         , const int &treeindex
//                         , QStringList variants
//                         , const int operation
//                         , QList<QList<QStringList> *> *cache
//                         , QUndoCommand *parent = nullptr);

    EditLocalVariantTree(const int &targetindex
                         , const int &treeindex
                         , const QString &localvariant
                         , const QString &localvalue
                         , const int operation
                         , EditorCacheList *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

//    QStringList m_newvar;
    VariantPair newVar;

private:
//    void updateIndex(int count);
//    void updateCounter(bool ascend);

    int targetIndex;
    int treeIndex;
//    QStringList m_oldvar;
    VariantPair oldVar;
    int tableOperation;

//    QList<QList<QStringList> *> *m_cache;
//    ProcessXmlListGenerator pxlg;

//    int SKIP;

    EditorCacheList *ptrCache;
};

#endif // EDITVARIANTTREE_H
