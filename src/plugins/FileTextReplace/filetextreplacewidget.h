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

#ifndef FILETEXTREPLACEWIDGET_H
#define FILETEXTREPLACEWIDGET_H

#include <QHash>
#include <pluginwidget.h>

namespace Ui {
class FileTextReplaceWidget;
}

class FileTextReplaceWidget : public PluginWidget
{
    Q_OBJECT

public:
    explicit FileTextReplaceWidget(QWidget *parent = nullptr);
    ~FileTextReplaceWidget() override;

public slots:
    void receiveList(QStringList args) override;

private slots:
    void on_inputEdit_textChanged(const QString &arg1);

    void on_outputEdit_textChanged(const QString &arg1);

    void on_searchEdit_textChanged(const QString &arg1);

    void on_replaceEdit_textChanged(const QString &arg1);

private:
    void updateArguments();

    Ui::FileTextReplaceWidget *ui;

    enum FileTextReplaceList{
        InputFile,
        OutputFile,
        SearchWord,
        ReplaceWord
    };

    QHash<FileTextReplaceList, QString> settingargs;
};

#endif // FILETEXTREPLACEWIDGET_H
