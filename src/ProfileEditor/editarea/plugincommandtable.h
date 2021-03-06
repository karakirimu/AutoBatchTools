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

#ifndef PLUGINCOMMANDTABLE_H
#define PLUGINCOMMANDTABLE_H

#include "commandtable.h"

class PluginCommandTable : public CommandTable
{
    Q_OBJECT
public:
    explicit PluginCommandTable(QWidget *parent = nullptr);
    ~PluginCommandTable() override;

    void updateTableList(QStringList *list);

signals:
    void updatePluginWidget(QStringList, int);

public slots:
    void insertSettingUpdate(QStringList list);

private slots:
    void addAction() override;
    void deleteAction() override;
    void cutAction() override;
    void pasteAction() override;
    void pasteSpaceAction() override;
    void pasteEnterAction() override;

    void editedAction(int row, int column) override;
};

#endif // PLUGINCOMMANDTABLE_H
