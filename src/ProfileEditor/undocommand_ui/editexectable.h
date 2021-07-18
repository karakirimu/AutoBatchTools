/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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

private:
    QString newStr;
    int index;
    int tableIndex;
    QString oldStr;
    int tableOperation;

    EditorCacheList *ptrCache;
};

#endif // EXECTABLEOPERATION_H
