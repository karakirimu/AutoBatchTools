/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STRINGPROJECTDESCRIPTION_H
#define STRINGPROJECTDESCRIPTION_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class StringProjectDescription : public QUndoCommand
{
public:
    StringProjectDescription(const int &targetindex
                             , const QString &newstring
                             , EditorCacheList *cache
                             , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString newStr;
private:
    int index;
    QString oldStr;

    EditorCacheList *ptrCache;
};

#endif // STRINGPROJECTDESCRIPTION_H
