/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
