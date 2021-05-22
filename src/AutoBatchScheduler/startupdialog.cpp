/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/default_icons/edit.png"));

    //install closeevent filter
    installEventFilter(this);

    ui->setupUi(this);

    //set input number only.
    ui->secondsLineEdit->setValidator(
        new QRegularExpressionValidator(QRegularExpression("[0-9]*"), this));

    //set button id
    ui->radioButtonGroup->setId(ui->oneshotRadioButton, 0);
    ui->radioButtonGroup->setId(ui->weeksRadioButton, 1);
    ui->radioButtonGroup->setId(ui->secondsRadioButton, 2);

    //connect action
    connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &StartupDialog::onAccept);
    connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &StartupDialog::onReject);
    connect(ui->everyDayCheckBox, &QAbstractButton::clicked, this, &StartupDialog::everyDaySelected);
    connect(ui->profileAddToolButton, &QToolButton::clicked,
            ui->profileComboBox, &ProfileComboBox::addItemAction);
    connect(ui->profileDeleteToolButton, &QToolButton::clicked,
            ui->profileComboBox, &ProfileComboBox::deleteItemAction);

    setWindowTitle(tr("Editing - Untitled*"));

    //init radiobutton
    ui->oneshotRadioButton->setChecked(true);

    //update combobox
    ui->profileComboBox->reloadComboBoxItem();

    //set current time
    QDateTime time = QDateTime::currentDateTime();
    ui->scheduleDateTimeEdit->setDateTime(time);
}

StartupDialog::~StartupDialog()
{
    delete ui;
}

void StartupDialog::load(const SchedulerCache &setting)
{
    settingCache = setting;

    //window title
    setWindowTitle(tr("Editing - ") + settingCache.settingName);

    //setting name
    ui->nameLineEdit->setText(settingCache.settingName);

    //setting prof
    ui->profileComboBox->setIndex(settingCache.profilePath);

    //set radio button
    ui->radioButtonGroup->button(static_cast<int>(settingCache.scheduleType)) \
                        ->setChecked(true);

    //set schedule datetime
    ui->scheduleDateTimeEdit->setDateTime(settingCache.oneShotDateTime);

    //set schedule seconds
    ui->secondsLineEdit->setText(secondsToTime(settingCache.periodicSeconds));

    //set schedule time
    ui->scheduleTimeEdit->setTime(settingCache.everyWeekTime);

    //set schedule day
    StringToDaySelect(settingCache.everyWeekDate);
}

void StartupDialog::onAccept()
{
    updateSetting();
    accept();
}

void StartupDialog::onReject()
{
    reject();
}

void StartupDialog::everyDaySelected(bool checked)
{
    ui->MonCheckBox->setChecked(checked);
    ui->TueCheckBox->setChecked(checked);
    ui->WedCheckBox->setChecked(checked);
    ui->ThuCheckBox->setChecked(checked);
    ui->FriCheckBox->setChecked(checked);
    ui->SatCheckBox->setChecked(checked);
    ui->SunCheckBox->setChecked(checked);
}

void StartupDialog::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton res = QMessageBox::question(\
      this, tr("Alert"), tr("Save this setting?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:
      onAccept();
      break;

    case QMessageBox::No:
      reject();
      break;

    case QMessageBox::Cancel:
      event->ignore();
      break;

    default:
      break;
    }
}

void StartupDialog::updateSetting()
{
    settingCache.settingName = ui->nameLineEdit->text();
    settingCache.profilePath = ui->profileComboBox->getCurrentFileName();
    settingCache.scheduleType = static_cast<ScheduleType>(ui->radioButtonGroup->checkedId());
    settingCache.oneShotDateTime = ui->scheduleDateTimeEdit->dateTime();
    settingCache.periodicSeconds = timeToSeconds(ui->secondsLineEdit->text()).toLongLong();
    settingCache.everyWeekTime = ui->scheduleTimeEdit->time();
    settingCache.everyWeekDate = daySelectToString();
}

QString StartupDialog::daySelectToString()
{
    QString tmp;

    tmp.append(ui->weeksSpinBox->text());
    tmp.append(QString::number(ui->MonCheckBox->isChecked()));
    tmp.append(QString::number(ui->TueCheckBox->isChecked()));
    tmp.append(QString::number(ui->WedCheckBox->isChecked()));
    tmp.append(QString::number(ui->ThuCheckBox->isChecked()));
    tmp.append(QString::number(ui->FriCheckBox->isChecked()));
    tmp.append(QString::number(ui->SatCheckBox->isChecked()));
    tmp.append(QString::number(ui->SunCheckBox->isChecked()));

    return tmp;
}

QString StartupDialog::timeToSeconds(QString data)
{
    //this is copy function.
    int comboindex = ui->secondsComboBox->currentIndex();
    int num = data.toInt();

    switch(comboindex){
    case 0:            break;
    case 1: num*=60;   break;
    case 2: num*=3600; break;
    default:           break;
    }

    return QString::number(num);
}

QString StartupDialog::secondsToTime(qint64 num)
{
    if(num % 3600 == 0){
        ui->secondsComboBox->setCurrentIndex(2);
        num/=3600;
    }else if(num % 60 == 0){
        ui->secondsComboBox->setCurrentIndex(1);
        num/=60;
    }else{
        ui->secondsComboBox->setCurrentIndex(0);
    }

    return QString::number(num);
}

void StartupDialog::StringToDaySelect(QString data)
{
    ui->weeksSpinBox->setValue(data.at(0).digitValue() == 1);
    ui->MonCheckBox->setChecked(data.at(1).digitValue() == 1);
    ui->TueCheckBox->setChecked(data.at(2).digitValue() == 1);
    ui->WedCheckBox->setChecked(data.at(3).digitValue() == 1);
    ui->ThuCheckBox->setChecked(data.at(4).digitValue() == 1);
    ui->FriCheckBox->setChecked(data.at(5).digitValue() == 1);
    ui->SatCheckBox->setChecked(data.at(6).digitValue() == 1);
    ui->SunCheckBox->setChecked(data.at(7).digitValue() == 1);

    //check datas
    if(ui->MonCheckBox->isChecked() &&
            ui->TueCheckBox->isChecked() &&
            ui->WedCheckBox->isChecked() &&
            ui->ThuCheckBox->isChecked() &&
            ui->FriCheckBox->isChecked() &&
            ui->SatCheckBox->isChecked() &&
            ui->SunCheckBox->isChecked() )
    {
        ui->everyDayCheckBox->setChecked(true);
    }
}
