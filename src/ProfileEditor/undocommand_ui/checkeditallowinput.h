/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef EDITALLOWINPUT_H
#define EDITALLOWINPUT_H

#include <QUndoCommand>
#include <uicommandmap.h>
#include <editorcachelist.h>

class CheckEditAllowInput : public QUndoCommand
{
public:
    CheckEditAllowInput(const int &targetindex
                   , const bool &newcheck
                   , EditorCacheList *cache
                   , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:

    int index;

    bool oldCheck;
    bool newCheck;

    EditorCacheList *ptrCache;
};

#endif // EDITALLOWINPUT_H
