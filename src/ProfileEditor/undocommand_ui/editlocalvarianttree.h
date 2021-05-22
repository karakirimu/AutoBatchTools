/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITVARIANTTREE_H
#define EDITVARIANTTREE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>
#include <QDebug>

class EditLocalVariantTree : public QUndoCommand
{
public:
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

private:
    int targetIndex;
    int treeIndex;
    VariantPair oldVar;
    VariantPair newVar;
    int tableOperation;

    EditorCacheList *ptrCache;
};

#endif // EDITVARIANTTREE_H
