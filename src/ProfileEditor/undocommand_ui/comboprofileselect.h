/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef COMBOPROFILESELECT_H
#define COMBOPROFILESELECT_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class ComboProfileSelect : public QUndoCommand
{
public:
    ComboProfileSelect(const int &targetindex
                       , const QString newstring
                       , const QString newfile
                       , EditorCacheList *cache
                       , QUndoCommand *parent = nullptr);

   void undo() override;
   void redo() override;

   int id() const override;
   bool mergeWith(const QUndoCommand *other) override;

   QString newString;
   QString newFile;

private:
   int index;
   QString oldFile;

   EditorCacheList *ptrCache;

};

#endif // COMBOPROFILESELECT_H
