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

#ifndef TAKELINEWIDGET_H
#define TAKELINEWIDGET_H

#include <QHash>
#include <QWidget>
#include <pluginwidget.h>

namespace Ui {
class TakeLineWidget;
}

class TakeLineWidget : public PluginWidget
{
    Q_OBJECT

public:
    explicit TakeLineWidget(QWidget *parent = nullptr);
    ~TakeLineWidget() override;

public slots:
    void receiveList(QStringList args) override;

private slots:
    void on_plainTextEdit_textChanged();

    void on_removelineEdit_textChanged(const QString &arg1);

    void on_returnLineEdit_textChanged(const QString &arg1);

private:
    void updateArguments();

    Ui::TakeLineWidget *ui;

    enum TakeLineList{
        SourceOrString,
        RemoveLineNumber,
        LocalVariant
    };

    QHash<TakeLineList,QString> settingargs;
};

#endif // TAKELINEWIDGET_H
