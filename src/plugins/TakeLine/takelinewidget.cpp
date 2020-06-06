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

#include "takelinewidget.h"
#include "ui_takelinewidget.h"

TakeLineWidget::TakeLineWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::TakeLineWidget)
{
    ui->setupUi(this);
}

TakeLineWidget::~TakeLineWidget()
{
    delete ui;
}

void TakeLineWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    if(counter > 0) ui->plainTextEdit->setPlainText(args.at(TakeLineList::SourceOrString));
    if(counter > 1) ui->removelineEdit->setText(args.at(TakeLineList::RemoveLineNumber));
    if(counter > 2) ui->returnLineEdit->setText(args.at(TakeLineList::LocalVariant));


    this->blockSignals(false);
}

void TakeLineWidget::updateArguments()
{
    QStringList converted;

    converted.append(settingargs.value(TakeLineList::SourceOrString, ""));
    converted.append(settingargs.value(TakeLineList::RemoveLineNumber, ""));
    converted.append(settingargs.value(TakeLineList::LocalVariant, ""));

    sendList(converted);
}

void TakeLineWidget::on_plainTextEdit_textChanged()
{
    settingargs.insert(TakeLineList::SourceOrString, ui->plainTextEdit->toPlainText());
    updateArguments();
}

void TakeLineWidget::on_removelineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(TakeLineList::RemoveLineNumber, arg1);
    updateArguments();
}

void TakeLineWidget::on_returnLineEdit_textChanged(const QString &arg1)
{
    settingargs.insert(TakeLineList::LocalVariant, arg1);
    updateArguments();
}
