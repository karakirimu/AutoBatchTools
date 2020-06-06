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

#include "removedirectorywidget.h"
#include "ui_removedirectorywidget.h"

RemoveDirectoryWidget::RemoveDirectoryWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::RemoveDirectryWidget)
{
    ui->setupUi(this);
}

RemoveDirectoryWidget::~RemoveDirectoryWidget()
{
    delete ui;
}

void RemoveDirectoryWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    // reset ui
    ui->rmpathLineEdit->clear();
    ui->fileRemoveCheckBox->setChecked(false);

    if(counter > 0) ui->rmpathLineEdit->setText(args.at(RemoveDirectoryList::RemovePath));
    if(counter > 1 && args.at(RemoveDirectoryList::FileInclude) == "-rf") ui->fileRemoveCheckBox->setChecked(true);


    this->blockSignals(false);
}

void RemoveDirectoryWidget::updateArguments()
{
    QStringList converted;
    converted.append(settingargs.value(RemoveDirectoryList::RemovePath, ""));
    converted.append(settingargs.value(RemoveDirectoryList::FileInclude, ""));

    sendList(converted);
}

void RemoveDirectoryWidget::on_rmpathLineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(RemoveDirectoryList::RemovePath, arg1);
    updateArguments();
}

void RemoveDirectoryWidget::on_fileRemoveCheckBox_stateChanged(int arg1)
{
    if(arg1 == Qt::CheckState::Checked){
        settingargs.insert(RemoveDirectoryList::FileInclude, "-rf");
    }else{
        settingargs.insert(RemoveDirectoryList::FileInclude, "");
    }
    updateArguments();
}
