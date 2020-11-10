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

#include "cellinfowidget.h"
#include "ui_cellinfowidget.h"

CellInfoWidget::CellInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellInfoWidget)
{
    ui->setupUi(this);

    //init
    ui->progressBar->setValue(0);
    ui->progressBar->setAlignment(Qt::AlignCenter);
    connect(ui->consoleToolButton, &QToolButton::clicked, this, &CellInfoWidget::onConsoleButtonClicked);
    connect(ui->pauseToolButton, &QToolButton::clicked, this, &CellInfoWidget::onPauseButtonClicked);
    connect(ui->stopToolButton, &QToolButton::clicked, this, &CellInfoWidget::onStopButtonClicked);

    connect(ui->profileCheckBox, &QCheckBox::toggled, this, &CellInfoWidget::onRunStatusChange);

    //hide console
    ui->consolearea->hide();

}

CellInfoWidget::~CellInfoWidget()
{
    delete ui;
}

int CellInfoWidget::indicateHeight()
{
    return ui->indicatearea->baseSize().height();
}

void CellInfoWidget::setConsoleTarget(TaskSchedulerConnector *taskc)
{
    ui->console->setTaskSchedulerConnector(taskc);
    ui->consolesender->setTaskSchedulerConnector(taskc);
    ui->console->setReadObjectName(this->objectName());
    ui->consolesender->setObjectName(this->objectName());
}

bool CellInfoWidget::isConsoleVisible()
{
    return ui->consolearea->isVisible();
}

void CellInfoWidget::setConsoleVisible(bool show)
{
    ui->consolearea->setVisible(show);
}

QHash<int, QVariant> CellInfoWidget::getState() const
{
    QHash<int, QVariant> saved;
    saved.insert(UpdateText, ui->currentProcessLabel->text());
    saved.insert(TimerText, ui->nextTimeLabel->text());
//    saved.insert(Progress, ui->progressBar->value());
    saved.insert(ConsoleText, ui->console->toHtml());
    saved.insert(ProgressEnable, ui->progressBar->isEnabled());
    saved.insert(PauseEnable, ui->pauseToolButton->isEnabled());
    saved.insert(StopEnable, ui->stopToolButton->isEnabled());
    saved.insert(ConsoleVisible, ui->consolearea->isVisible());

    return saved;
}

void CellInfoWidget::restoreState(const QHash<int, QVariant> &state)
{
    ui->currentProcessLabel->setText(state[UpdateText].toString());
    ui->nextTimeLabel->setText(state[TimerText].toString());
//    ui->progressBar->setValue(state[Progress].toInt());
    ui->console->setHtml(state[ConsoleText].toString());
    ui->progressBar->setEnabled(state[ProgressEnable].toBool());
    ui->pauseToolButton->setEnabled(state[PauseEnable].toBool());
    ui->stopToolButton->setEnabled(state[StopEnable].toBool());
    ui->consolearea->setVisible(state[ConsoleVisible].toBool());
}

void CellInfoWidget::setProfileName(QString name)
{
    ui->profileCheckBox->setText(name);
}

void CellInfoWidget::started()
{
    ui->progressBar->setEnabled(true);
    ui->pauseToolButton->setEnabled(true);
    ui->stopToolButton->setEnabled(true);
}

void CellInfoWidget::scheduled()
{
    ui->progressBar->setEnabled(false);
    ui->pauseToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
}

void CellInfoWidget::updateProcess(QString data, int type)
{
    Q_UNUSED(type)
    ui->currentProcessLabel->setText(data);
}

void CellInfoWidget::updateProgress(int num)
{
    ui->progressBar->setValue(num);
}

void CellInfoWidget::updateErrorProgress(int num)
{
    QPalette p = ui->progressBar->palette();
    p.setColor(QPalette::Highlight, Qt::darkRed);
    ui->progressBar->setPalette(p);
    ui->progressBar->repaint();
    ui->progressBar->setValue(num);
}

void CellInfoWidget::updateErrorText(QString message)
{
    ui->currentProcessLabel->setText(message);
}

void CellInfoWidget::setRunStatus(bool enabled)
{
    ui->profileCheckBox->setChecked(enabled);
    ui->nextTimeLabel->clear();
}

void CellInfoWidget::onRunStatusChange(bool enabled)
{
    if(ui->stopToolButton->isEnabled()){
        //task is running
        blockSignals(true);
        ui->profileCheckBox->setChecked(true);
        ui->currentProcessLabel->clear();
        ui->progressBar->setValue(0);
        blockSignals(false);

        //TEXT
        updateErrorText(tr("To turn off task, need to stop the current task."));
    }else{
        //task is not running
        setRunStatus(enabled);
        emit changeRunStatus(enabled);
    }
}

void CellInfoWidget::setTimerEnd(const QString &date)
{
    qDebug() << "[CellInfoWidget::setTimerEnd] text : " << date;
    ui->nextTimeLabel->setText(date);
}

void CellInfoWidget::setProgressminmax(int start, int end)
{
    ui->progressBar->setRange(start, end);
}

void CellInfoWidget::onConsoleButtonClicked()
{
    emit consoleButtonClicked(this->objectName());
}

void CellInfoWidget::onPauseButtonClicked()
{
    emit pauseButtonClicked(this->objectName());
}

void CellInfoWidget::onStopButtonClicked()
{
    emit stopButtonClicked(this->objectName());
}
