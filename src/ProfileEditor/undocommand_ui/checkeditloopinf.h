/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITLOOPINFCHECK_H
#define EDITLOOPINFCHECK_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class CheckEditLoopInf : public QUndoCommand
{
public:
    CheckEditLoopInf(const int &targetindex
                     , const bool &newcheck
                     , EditorCacheList *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
   int index;

   bool oldCheck;
   bool newCheck;

   EditorCacheList *ptrCache;
};

#endif // EDITLOOPINFCHECK_H
