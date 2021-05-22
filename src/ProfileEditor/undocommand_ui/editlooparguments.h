/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITLOOPARGUMENTS_H
#define EDITLOOPARGUMENTS_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class EditLoopArguments : public QUndoCommand
{
public:
    EditLoopArguments(const int &targetindex
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

#endif // EDITLOOPARGUMENTS_H
