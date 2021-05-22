/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef COMBOPLUGINSELECT_H
#define COMBOPLUGINSELECT_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class ComboPluginSelect : public QUndoCommand
{
public:
    ComboPluginSelect(const int &targetindex
                      , const QString newstring
                      , const QString newfile
                      , EditorCacheList *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
//    bool mergeWith(const QUndoCommand *other) override;

private:
    int index;

    QString oldString;
    QString oldFile;
    QStringList oldCommand;

    QString newString;
    QString newFile;

    EditorCacheList *ptrCache;
};

#endif // COMBOPLUGINSELECT_H
