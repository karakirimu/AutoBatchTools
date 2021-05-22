/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITSEARCHOUTPUT_H
#define EDITSEARCHOUTPUT_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class EditSearchOutput : public QUndoCommand
{
public:
    EditSearchOutput(const int &targetindex
                     , const int &newvalue
                     , EditorCacheList *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    int newValue;

private:
    void replaceValue(int value);

    int index;
    int oldValue;

    EditorCacheList *ptrCache;

};

#endif // EDITSEARCHOUTPUT_H
