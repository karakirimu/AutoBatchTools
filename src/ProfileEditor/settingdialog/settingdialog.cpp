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

#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    ui->setupUi(this);
    setupItem();

    //connect action
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingDialog::onButtonBoxClicked);
    connect(ui->openToolButton, &QToolButton::clicked, this, &SettingDialog::openDir);

    //search variant
    connect(ui->seAddButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::addAction);
    connect(ui->seCopyButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::copyAction);
    connect(ui->seDeleteButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::deleteAction);
    connect(ui->seDownButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::upAction);
    connect(ui->seUpButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::downAction);

    //extrafunc variant
    connect(ui->plAddButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::addAction);
    connect(ui->plDeleteButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::deleteAction);
    connect(ui->plUpButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::upAction);
    connect(ui->plDownButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::downAction);

}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setListPos(int num)
{
    //todo: whether slot is run or not.
    ui->listWidget->item(num)->setSelected(true);
    ui->stackedWidget->setCurrentIndex(num);
    ui->stackedWidget->setFocus();
    if(num == 2) ui->searchTableWidget->setStyleSheet(this->styleSheet());
}

void SettingDialog::setupItem(){
    ui->listWidget->addItem(tr("General"));
    ui->listWidget->addItem(tr("Test"));
    ui->listWidget->addItem(tr("Search"));
    ui->listWidget->addItem(tr("Plugins"));

    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setIconSize(QSize(32,32));

    ui->listWidget->item(0)->setIcon(QIcon(":/default_icons/settings.png"));
    ui->listWidget->item(1)->setIcon(QIcon(":/default_icons/string.png"));
    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/search.png"));
    ui->listWidget->item(3)->setIcon(QIcon(":/default_icons/extras.png"));
}

void SettingDialog::setSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("pe_general");
    settings.setValue("TEMPDIR", ui->tempEdit->text());
    settings.setValue("AUTOSAVEPERIOD", ui->autosaveSpinBox->value());
    settings.setValue("THEMECOLOR", ui->themeComboBox->currentText());
    settings.setValue("WINDOWFONT", ui->windowFontComboBox->currentText());
    settings.setValue("WINDOWFONTSIZE", ui->windowFontSizeSpinBox->value());

    settings.beginGroup("pe_testexec");
    settings.setValue("FAKERES", ui->fakeresidentCheckBox->isChecked());
    settings.endGroup();
}

void SettingDialog::loadSettings()
{
    QSettings settings( "./settings.ini", QSettings::IniFormat );

    settings.beginGroup("pe_general");
    ui->tempEdit->setText(settings.value("TEMPDIR", "./").toString());
    ui->autosaveSpinBox->setValue(settings.value("AUTOSAVEPERIOD", 5).toInt());
    ui->themeComboBox->setCurrentText(settings.value("THEMECOLOR", "Default").toString());
    ui->windowFontComboBox->setCurrentFont(QFont(settings.value("WINDOWFONT", QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(settings.value("WINDOWFONTSIZE", QApplication::font().pointSize()).toInt());
    settings.endGroup();

    settings.beginGroup("pe_testexec");
    ui->fakeresidentCheckBox->setChecked(settings.value("FAKERES", false).toBool());
    settings.endGroup();

}

//change list to stackedwidget
void SettingDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
    if(currentRow == 2) ui->searchTableWidget->setStyleSheet(this->styleSheet());
}

void SettingDialog::onButtonBoxClicked(QAbstractButton *button)
{
    QString pressed = button->text();
    if(pressed == "Apply"
            || pressed == "OK"){
        setSettings();
    }
}

void SettingDialog::openDir()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    QString res = dialog.getExistingDirectory(this, tr("Open Folder"), ui->tempEdit->text());
    ui->tempEdit->setText(res);
}

void SettingDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    loadSettings();
}

void SettingDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    this->hide();
}
