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

#include "settingwidget.h"
#include "ui_settingwidget.h"

SettingWidget::SettingWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::SettingWidget)
{
    ui->setupUi(this);
}

void SettingWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    // reset ui
    ui->lineEditSource->clear();
    ui->lineEditDestination->clear();
    ui->checkBoxRename->setChecked(false);
    ui->lineEditRename->clear();
    ui->checkBoxCache->setChecked(false);
    ui->lineEditCache->clear();
    ui->checkBoxVerify->setChecked(false);

    if(counter > 0) ui->lineEditSource->setText(args.at(SettingList::Source));
    if(counter > 1) ui->lineEditDestination->setText(args.at(SettingList::Destination));

    QString argcheck;
    for(int i = 2; i < counter; i++){
        argcheck = static_cast<QString>(args.at(i)).toLower();

        if(argcheck == "-rename" && (i + 1) < counter){
            ui->checkBoxRename->setChecked(true);
            ui->lineEditRename->setText(args.at(i + 1));
            i++;
        }

        if(argcheck == "-cs"){
            ui->checkBoxCache->setChecked(true);

            if((i + 1) < counter && args.at(i + 1) != "-v"){
                ui->lineEditCache->setText(args.at(i + 1));
                i++;
            }
        }

        if(argcheck == "-v") ui->checkBoxVerify->setChecked(true);
    }

    this->blockSignals(false);
}

void SettingWidget::updateArguments()
{
    QStringList converted;

    converted.append(settingargs.value(SettingList::Source, ""));
    converted.append(settingargs.value(SettingList::Destination, ""));

    if(settingargs.value(SettingList::RenameCheck, "0").toInt() == Qt::Checked){
        converted.append("-rename");
        converted.append(settingargs.value(SettingList::Rename, ""));
    }

    if(settingargs.value(SettingList::CacheCheck, "0").toInt() == Qt::Checked){
        converted.append("-cs");
        if(!settingargs.value(SettingList::CacheSize).isEmpty()){
            converted.append(settingargs.value(SettingList::CacheSize, "524288"));
        }
    }

    if(settingargs.value(SettingList::HashCheck, "0").toInt() == Qt::Checked){
        converted.append("-v");
    }

    sendList(converted);
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::on_lineEditSource_textChanged(const QString &arg1)
{
    settingargs.insert(SettingList::Source, arg1);
    updateArguments();
}

void SettingWidget::on_lineEditDestination_textChanged(const QString &arg1)
{
    settingargs.insert(SettingList::Destination, arg1);
    updateArguments();
}

void SettingWidget::on_checkBoxRename_stateChanged(int arg1)
{
    settingargs.insert(SettingList::RenameCheck, QString::number(arg1));
    updateArguments();
}

void SettingWidget::on_lineEditRename_textChanged(const QString &arg1)
{
    settingargs.insert(SettingList::Rename, arg1);
    updateArguments();
}

void SettingWidget::on_checkBoxCache_stateChanged(int arg1)
{
    settingargs.insert(SettingList::CacheCheck, QString::number(arg1));
    updateArguments();
}

void SettingWidget::on_lineEditCache_textChanged(const QString &arg1)
{
    settingargs.insert(SettingList::CacheSize, arg1);
    updateArguments();
}

void SettingWidget::on_checkBoxVerify_stateChanged(int arg1)
{
    settingargs.insert(SettingList::HashCheck, QString::number(arg1));
    updateArguments();
}
