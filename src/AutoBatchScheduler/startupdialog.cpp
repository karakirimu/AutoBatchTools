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

#include "startupdialog.h"
#include "ui_startupdialog.h"

StartupDialog::StartupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupDialog)
{
    // delete ui on dialog closed
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/default_icons/edit.png"));

    //install closeevent filter
    installEventFilter(this);

    ui->setupUi(this);

    //set input number only.
    ui->secondsLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));

    //set button id
    ui->radioButtonGroup->setId(ui->oneshotRadioButton, 0);
    ui->radioButtonGroup->setId(ui->weeksRadioButton, 1);
    ui->radioButtonGroup->setId(ui->secondsRadioButton, 2);

    //connect action
    connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &StartupDialog::onAccept);
    connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &StartupDialog::onReject);
    connect(ui->everyDayCheckBox, &QAbstractButton::clicked, this, &StartupDialog::everyDaySelected);
    connect(ui->profileAddToolButton, &QToolButton::clicked, ui->profileComboBox, &ProfileComboBox::addItemAction);
    connect(ui->profileDeleteToolButton, &QToolButton::clicked, ui->profileComboBox, &ProfileComboBox::deleteItemAction);

    //set new xml builder
    builder = new StartupXmlBuilder();

    //init editflag
    editflag = false;
    uniquecode = "";

    //init radiobutton
    ui->oneshotRadioButton->setChecked(true);

    //update combobox
    ui->profileComboBox->reloadComboBoxItem();

    //set current time
    QDateTime time = QDateTime::currentDateTime();
    ui->scheduleDateTimeEdit->setDateTime(time);

//    qsrand(static_cast<uint>(time.currentSecsSinceEpoch() ^ 987465123));
}

StartupDialog::~StartupDialog()
{
    delete builder;
    delete ui;
}

void StartupDialog::loadSettingList(int index, const QList<QStringList> *data)
{
    qDebug() << data->toVector();
    if(data->count() != 9) return;

    //window title
    setWindowTitle(tr("Editing-") + data->at(0).at(1));

    //set edit flags
    editflag = true;
    //set edit index
    editindex = index;

    //setting data
    ui->nameLineEdit->setText(data->at(0).at(1));

    //setting prof
    ui->profileComboBox->setIndex(data->at(1).at(1));
    //int cindex = ui->profileComboBox->getIndexFromFileName(data->at(1).at(1));
//    if(cindex > -1){
//        ui->profileComboBox->setCurrentIndex(cindex);
//    }else{
//        ui->profileComboBox->setCurrentText(tr("Unknown"));
//    }

    //setting valid
//    ui->validCheckBox->setChecked(VariantConverter::stringToBool(data->at(2).at(1)));

    //set radio button
    ui->radioButtonGroup->button((static_cast<QString>(data->at(3).at(1)).toInt()))->setChecked(true);

    //set schedule datetime
    QDateTime datetime = QDateTime::fromString(data->at(4).at(1), "yyyy/MM/dd HH:mm:ss");
    ui->scheduleDateTimeEdit->setDateTime(datetime);

    //set schedule seconds
    ui->secondsLineEdit->setText(secondsToTime(data->at(5).at(1)));

    //set schedule time
    QTime time = QTime::fromString(data->at(6).at(1), "HH:mm:ss");
    ui->scheduleTimeEdit->setTime(time);

    //set schedule day
    StringToDaySelect(data->at(7).at(1));

    //set unique code
    uniquecode = data->at(8).at(1);

}

void StartupDialog::onAccept()
{
    QList<QStringList> list;
    createList(&list);
    if(editflag){
        // modified
        builder->editItem(editindex, &list);
    }else{
        builder->addItem(&list);
    }
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
    // ドキュメントが変更されている場合の警告
    QMessageBox::StandardButton res = QMessageBox::question(\
      this, tr("Alert"), tr("Overwrite this file ?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    switch( res )
    {
    case QMessageBox::Yes:
      // 保存できたなら、閉じてOK
      onAccept();
      //if (saved())  event->accept();
      break;

    case QMessageBox::No:
      // 保存しなくて、そのまま閉じてOK
      reject();
      break;

    case QMessageBox::Cancel:
      // キャンセルして作業に戻る
      event->ignore();
      break;

    default:
      break;
    }
}

void StartupDialog::createList(QList<QStringList> *newlist)
{
    QStringList tmp;

    //add name
    newlist->append((QStringList() << "name" << ui->nameLineEdit->text()));

    //add profilename
    newlist->append((QStringList() << "prof" << ui->profileComboBox->getCurrentFileName()));

    //add setting is valid
    newlist->append((QStringList() << "valid" << VariantConverter::boolToString(false)));

    //add radiobutton select
    newlist->append((QStringList() << "radio" << QString::number(ui->radioButtonGroup->checkedId())));

    //add schedule datetime
    newlist->append((QStringList() << "schdt" << ui->scheduleDateTimeEdit->text()));

    //add schedule seconds
    newlist->append((QStringList() << "schsec" << timeToSeconds(ui->secondsLineEdit->text())));

    //add schedule time
    newlist->append((QStringList() << "scht" << ui->scheduleTimeEdit->text()));

    //add schedule days
    newlist->append((QStringList() << "schday" << daySelectToString()));

    //add unique token (for manage)
    QString token = editflag ? uniquecode : getRandomString(32);
    newlist->append((QStringList() << "unique" << token));

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

QString StartupDialog::getRandomString(int length)
{
    QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.");

    //shuffle characters
    int pos = possibleCharacters.size() - 1;
    int random;
    while(pos > 1){
        random = static_cast<int>(QRandomGenerator::global()->generate()) % pos;
        QChar tmp = possibleCharacters.at(random);
        possibleCharacters.replace(random, 1, possibleCharacters.at(pos));
        possibleCharacters.replace(pos, 1, tmp);
        pos--;
    }

    //select characters
    QString randomString;
    for(int i=0; i < length; ++i)
    {
        int index = static_cast<int>(QRandomGenerator::global()->generate()) % length;
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

QString StartupDialog::timeToSeconds(QString data)
{
    //this is copy function.
    int comboindex = ui->secondsComboBox->currentIndex();
    int num = data.toInt();

    switch(comboindex){
    case 0:
        break;
    case 1:
        num*=60;
        break;
    case 2:
        num*=3600;
        break;
    default:
        break;
    }

    return QString::number(num);
}

QString StartupDialog::secondsToTime(QString data)
{
    //this is copy function.
    int num = data.toInt();

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
