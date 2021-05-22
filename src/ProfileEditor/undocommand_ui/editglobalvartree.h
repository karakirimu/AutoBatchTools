/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITGLOBALVARTREE_H
#define EDITGLOBALVARTREE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <uicommandmap.h>

/**
 * @brief The EditGlobalVarTree class
 * variables structure
 * 0       :1       :
 * variable :value   :
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

private:
    int m_treeindex;
    int m_tableindex;
    QStringList m_newvar;
    QStringList m_oldvar;
    int m_operation;

    StringXmlBuilder sxml;
    EditorCacheList *ptrCache;
};

#endif // EDITGLOBALVARTREE_H
