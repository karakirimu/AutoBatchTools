/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITSEARCHFILEOUTPUTTYPE_H
#define EDITSEARCHFILEOUTPUTTYPE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class EditSearchFileOutputType : public QUndoCommand
{
public:
    EditSearchFileOutputType(const int &targetindex
                             , const int &newvalue
                             , EditorCacheList *cache
                             , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

private:
    void replaceValue(int value);

    int index;
    int oldValue;
    int newValue;

    EditorCacheList *ptrCache;
};

#endif // EDITSEARCHFILEOUTPUTTYPE_H
