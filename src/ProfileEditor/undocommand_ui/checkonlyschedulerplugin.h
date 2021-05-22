/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef CHECKONLYSCHEDULERPLUGIN_H
#define CHECKONLYSCHEDULERPLUGIN_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class CheckOnlySchedulerPlugin : public QUndoCommand
{
public:
    CheckOnlySchedulerPlugin(const int &targetindex
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

#endif // CHECKONLYSCHEDULERPLUGIN_H
