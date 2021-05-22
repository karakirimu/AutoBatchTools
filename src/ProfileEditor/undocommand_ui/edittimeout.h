/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITTIMEOUT_H
#define EDITTIMEOUT_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class EditTimeout : public QUndoCommand
{
public:
    EditTimeout(const int &targetindex
                    , const int &newvalue
                    , EditorCacheList *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    int newValue;

private:
    int index;
    int oldValue;

    EditorCacheList *ptrCache;

};

#endif // EDITTIMEOUT_H
