/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SWAPGLOBALVARTREE_H
#define SWAPGLOBALVARTREE_H

#include <QUndoCommand>
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <uicommandmap.h>

class SwapGlobalVarTree : public QUndoCommand
{
public:
    SwapGlobalVarTree(const int &before
                      , const int &after
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    void setTextByCondition(int source, int dest);

    int indexBefore;
    int indexAfter;
    QString m_objname;

    StringXmlBuilder sxml;
};

#endif // SWAPGLOBALVARTREE_H
