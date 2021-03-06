/*
 * Copyright 2016-2021 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SWAPPLUGINTABLE_H
#define SWAPPLUGINTABLE_H

#include <QUndoCommand>
#include <editorcachelist.h>
#include <uicommandmap.h>

class SwapPluginTable : public QUndoCommand
{
public:
    SwapPluginTable(const int &targetindex
                    , const int &tablebefore
                    , const int &tableafter
                    , EditorCacheList *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    int id() const override;

private:
    void replaceValue(int source, int dest);
    void setTextByCondition(int source, int dest);

    int index;
    int indexBefore;
    int indexAfter;

    EditorCacheList *ptrCache;
};

#endif // SWAPPLUGINTABLE_H
