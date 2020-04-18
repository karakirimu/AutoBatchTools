/*
 * Copyright 2016-2020 karakirimu
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

#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QHash>
#include "../plugininterface/pluginwidget.h"

namespace Ui {
class SettingWidget;
}

class SettingWidget : public PluginWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget() override;

public slots:
    void receiveList(QStringList args) override;

private slots:
    void on_lineEditSource_textChanged(const QString &arg1);

    void on_lineEditDestination_textChanged(const QString &arg1);

    void on_checkBoxRename_stateChanged(int arg1);

    void on_lineEditRename_textChanged(const QString &arg1);

    void on_checkBoxCache_stateChanged(int arg1);

    void on_lineEditCache_textChanged(const QString &arg1);

    void on_checkBoxVerify_stateChanged(int arg1);

private:
    void updateArguments();

    Ui::SettingWidget *ui;

    enum SettingList{
        Source,
        Destination,
        CacheCheck,
        CacheSize,
        RenameCheck,
        Rename,
        HashCheck
    };

    QHash<SettingList,QString> settingargs;
};

#endif // SETTINGWIDGET_H
