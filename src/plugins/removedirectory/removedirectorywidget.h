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

#ifndef REMOVEDIRECTRYWIDGET_H
#define REMOVEDIRECTRYWIDGET_H

#include "pluginwidget.h"
#include <QHash>

namespace Ui {
class RemoveDirectoryWidget;
}

class RemoveDirectoryWidget : public PluginWidget
{
    Q_OBJECT

public:
    explicit RemoveDirectoryWidget(QWidget *parent = nullptr);
    ~RemoveDirectoryWidget() override;

public slots:
    void receiveList(QStringList args) override;

private slots:
    void on_rmpathLineEdit_textChanged(const QString &arg1);

    void on_fileRemoveCheckBox_stateChanged(int arg1);

private:
    void updateArguments();

    Ui::RemoveDirectoryWidget *ui;

    enum RemoveDirectoryList{
        RemovePath,
        FileInclude
    };

    QHash<RemoveDirectoryList, QString> settingargs;
};

#endif // REMOVEDIRECTRYWIDGET_H
