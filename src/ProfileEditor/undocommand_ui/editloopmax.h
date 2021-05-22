/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITLOOPMAX_H
#define EDITLOOPMAX_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class EditLoopMax : public QUndoCommand
{
public:
    EditLoopMax(const int &targetindex
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

#endif // EDITLOOPMAX_H
