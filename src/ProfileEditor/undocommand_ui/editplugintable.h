/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EXTRATABLECOMMAND_H
#define EXTRATABLECOMMAND_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditPluginTable : public QUndoCommand
{
public:
    EditPluginTable(const int &targetindex
                      ,const int &tableindex
                      ,const QString newstr
                      ,const int operation
                      ,EditorCacheList *cache
                      ,QUndoCommand *parent = nullptr);

    EditPluginTable(const int &targetindex
                     , const QStringList newstrlist
                     , const int operation
                     , EditorCacheList *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newStr;
    QStringList newStrList;

private:
    int index;
    int tableIndex;
    QString oldStr;
    QStringList oldStrList;
    int tableOperation;

    EditorCacheList *ptrCache;
};

#endif // EXTRATABLECOMMAND_H
