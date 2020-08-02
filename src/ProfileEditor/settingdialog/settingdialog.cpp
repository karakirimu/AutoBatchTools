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

    //langcombobox
    initLanguageSelection();

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
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABE);
    settings.setValue(sc.ABE_AUTOSAVE_DIR, ui->tempEdit->text());
    settings.setValue(sc.ABE_AUTOSAVE_PERIOD, ui->autosaveSpinBox->value());
    settings.setValue(sc.ABE_THEME, ui->themeComboBox->currentText());
    settings.setValue(sc.ABE_FONT, ui->windowFontComboBox->currentText());
    settings.setValue(sc.ABE_FONTSIZE, ui->windowFontSizeSpinBox->value());
    storeLanguageSelection(&settings);
    settings.setValue(sc.ABE_RUNAS_SCHEDULER, ui->fakeresidentCheckBox->isChecked());
    settings.endGroup();
}

void SettingDialog::loadSettings()
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABE);
    ui->tempEdit->setText(settings.value(sc.ABE_AUTOSAVE_DIR, "./").toString());
    ui->autosaveSpinBox->setValue(settings.value(sc.ABE_AUTOSAVE_PERIOD, 1).toInt());
    ui->themeComboBox->setCurrentText(settings.value(sc.ABE_THEME, "Default").toString());
    ui->windowFontComboBox->setCurrentFont(QFont(settings.value(sc.ABE_FONT, QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(settings.value(sc.ABE_FONTSIZE, QApplication::font().pointSize()).toInt());
    loadLanguageSelection(&settings);
    ui->fakeresidentCheckBox->setChecked(settings.value(sc.ABE_RUNAS_SCHEDULER, false).toBool());
    settings.endGroup();

}

void SettingDialog::initLanguageSelection()
{
#ifdef QT_DEBUG
    QDirIterator dit("../../src/translation", QStringList() << "*.qm", QDir::Files);
#else
    QDirIterator dit("translation", QStringList() << "*.qm", QDir::Files);
#endif

    QStringList files;

    while (dit.hasNext()){
        QFileInfo file(dit.next());
        QString strloc = file.baseName().split("_").last();
        QLocale locale(strloc);
        ui->languageComboBox->addItem(QLocale::languageToString(locale.language()));
    }
}

void SettingDialog::storeLanguageSelection(QSettings *setting)
{
    QString selected = ui->languageComboBox->currentText();

    QList<QLocale> locales = QLocale::matchingLocales(QLocale::AnyLanguage,
                                                      QLocale::AnyScript,
                                                      QLocale::AnyCountry);

    for(QLocale loc : locales){
        if(QLocale::languageToString(loc.language()) == selected){
            setting->setValue(sc.ABE_LANGUAGE, loc.bcp47Name());
            break;
        }
    }
}

void SettingDialog::loadLanguageSelection(QSettings *setting)
{
    QLocale defloc;
    QLocale locale(setting->value(sc.ABE_LANGUAGE, defloc.bcp47Name()).toString());
    ui->languageComboBox->setCurrentText(QLocale::languageToString(locale.language()));
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
    if(res != "") ui->tempEdit->setText(res);
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
