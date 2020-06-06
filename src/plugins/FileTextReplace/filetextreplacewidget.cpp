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

#include "filetextreplacewidget.h"
#include "ui_filetextreplacewidget.h"

FileTextReplaceWidget::FileTextReplaceWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::FileTextReplaceWidget)
{
    ui->setupUi(this);
}

FileTextReplaceWidget::~FileTextReplaceWidget()
{
    delete ui;
}

void FileTextReplaceWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    // reset ui
    ui->inputEdit->clear();
    ui->outputEdit->clear();
    ui->searchEdit->clear();
    ui->replaceEdit->clear();

    if(counter > 0) ui->inputEdit->setText(args.at(FileTextReplaceList::InputFile));
    if(counter > 1) ui->outputEdit->setText(args.at(FileTextReplaceList::OutputFile));
    if(counter > 2) ui->searchEdit->setText(args.at(FileTextReplaceList::SearchWord));
    if(counter > 3) ui->replaceEdit->setText(args.at(FileTextReplaceList::ReplaceWord));

    this->blockSignals(false);
}

void FileTextReplaceWidget::updateArguments()
{
    QStringList converted;

    converted.append(settingargs.value(FileTextReplaceList::InputFile, ""));
    converted.append(settingargs.value(FileTextReplaceList::OutputFile, ""));
    converted.append(settingargs.value(FileTextReplaceList::SearchWord, ""));
    converted.append(settingargs.value(FileTextReplaceList::ReplaceWord, ""));

    sendList(converted);
}

void FileTextReplaceWidget::on_inputEdit_textChanged(const QString &arg1)
{
    settingargs.insert(FileTextReplaceList::InputFile, arg1);
    updateArguments();
}

void FileTextReplaceWidget::on_outputEdit_textChanged(const QString &arg1)
{
    settingargs.insert(FileTextReplaceList::OutputFile, arg1);
    updateArguments();
}

void FileTextReplaceWidget::on_searchEdit_textChanged(const QString &arg1)
{
    settingargs.insert(FileTextReplaceList::SearchWord, arg1);
    updateArguments();
}

void FileTextReplaceWidget::on_replaceEdit_textChanged(const QString &arg1)
{
    settingargs.insert(FileTextReplaceList::ReplaceWord, arg1);
    updateArguments();
}
