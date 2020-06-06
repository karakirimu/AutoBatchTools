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

#include "regexpinputrenamewidget.h"
#include "ui_regexpinputrenamewidget.h"

RegExpInputRenameWidget::RegExpInputRenameWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::RegExpInputRenameWidget)
{
    ui->setupUi(this);
}

RegExpInputRenameWidget::~RegExpInputRenameWidget()
{
    delete ui;
}

void RegExpInputRenameWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    // reset ui
    ui->InputlineEdit->clear();
    ui->regExplineEdit->clear();
    ui->replaceLineEdit->clear();
    ui->separatorLineEdit->clear();

    if(counter > 0) ui->InputlineEdit->setText(args.at(RegExpInputRenameList::Input));
    if(counter > 1) ui->regExplineEdit->setText(args.at(RegExpInputRenameList::RegExp));
    if(counter > 2) ui->replaceLineEdit->setText(args.at(RegExpInputRenameList::Replace));
    if(counter > 3) ui->separatorLineEdit->setText(args.at(RegExpInputRenameList::Separator));

    this->blockSignals(false);
}

void RegExpInputRenameWidget::updateArguments()
{
    QStringList converted;

    converted.append(settingargs.value(RegExpInputRenameList::Input, ""));
    converted.append(settingargs.value(RegExpInputRenameList::RegExp, ""));
    converted.append(settingargs.value(RegExpInputRenameList::Replace, ""));
    converted.append(settingargs.value(RegExpInputRenameList::Separator, ""));

    sendList(converted);
}

void RegExpInputRenameWidget::on_InputlineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(RegExpInputRenameList::Input, arg1);
    updateArguments();
}

void RegExpInputRenameWidget::on_regExplineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(RegExpInputRenameList::RegExp, arg1);
    updateArguments();
}

void RegExpInputRenameWidget::on_replaceLineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(RegExpInputRenameList::Replace, arg1);
    updateArguments();
}

void RegExpInputRenameWidget::on_separatorLineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(RegExpInputRenameList::Separator, arg1);
    updateArguments();
}
