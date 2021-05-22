/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITLOCALVARTABLE_H
#define EDITLOCALVARTABLE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>
#include <QDebug>

class EditLocalVarTable : public QUndoCommand
{
public:
    EditLocalVarTable(const int &targetindex
                      , const int &tableindex
                      , const QString newVariant
                      , const QString newValue
                      , const int operation
                      , EditorCacheList *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

    VariantPair newVar;

private:
    int index;
    int tableIndex;
    int tableOperation;

    VariantPair oldVar;
    EditorCacheList *ptrCache;
};

#endif // EDITLOCALVARTABLE_H
