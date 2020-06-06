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

#include "ffprobeextpidwidget.h"
#include "ui_ffprobeextpidwidget.h"

FFprobeExtPidWidget::FFprobeExtPidWidget(QWidget *parent) :
    PluginWidget(parent),
    ui(new Ui::FFprobeExtPidWidget)
{
    ui->setupUi(this);
}

FFprobeExtPidWidget::~FFprobeExtPidWidget()
{
    delete ui;
}

void FFprobeExtPidWidget::updateArguments()
{
    QStringList converted;

    converted.append(settingargs.value(FFprobeExtPidList::InputFile, ""));
    converted.append(settingargs.value(FFprobeExtPidList::VideoPidFile, ""));
    converted.append(settingargs.value(FFprobeExtPidList::AudioPidFile, ""));

    sendList(converted);
}

void FFprobeExtPidWidget::receiveList(QStringList args)
{
    int counter = args.count();

    // block unexpected sendList signal
    this->blockSignals(true);

    // reset ui
    ui->input->clear();
    ui->out_videopid->clear();
    ui->out_audiopid->clear();

    if(counter > 0) ui->input->setText(args.at(FFprobeExtPidList::InputFile));
    if(counter > 1) ui->out_videopid->setText(args.at(FFprobeExtPidList::VideoPidFile));
    if(counter > 2) ui->out_audiopid->setText(args.at(FFprobeExtPidList::AudioPidFile));

    this->blockSignals(false);
}

void FFprobeExtPidWidget::on_input_textChanged(const QString &arg1)
{
    settingargs.insert(FFprobeExtPidList::InputFile, arg1);
    updateArguments();
}

void FFprobeExtPidWidget::on_out_videopid_textChanged(const QString &arg1)
{
    settingargs.insert(FFprobeExtPidList::VideoPidFile, arg1);
    updateArguments();
}

void FFprobeExtPidWidget::on_out_audiopid_textChanged(const QString &arg1)
{
    settingargs.insert(FFprobeExtPidList::AudioPidFile, arg1);
    updateArguments();
}
