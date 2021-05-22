/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITTIMEOUTCHECKBOX_H
#define EDITTIMEOUTCHECKBOX_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class CheckEditTimeout : public QUndoCommand
{
public:
    CheckEditTimeout(const int &targetindex
                        , const bool &newcheck
                        , EditorCacheList *cache
                        , QUndoCommand *parent = nullptr);


   void undo() override;
   void redo() override;

   int id() const override;

private:
   int index;

   bool newCheck;
   bool oldCheck;

   EditorCacheList *ptrCache;
};

#endif // EDITTIMEOUTCHECKBOX_H
