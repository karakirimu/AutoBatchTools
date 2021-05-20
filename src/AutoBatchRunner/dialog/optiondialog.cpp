/*
 * Copyright 2016-2021 karakirimu
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

#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    setupItem();

    //connect action
    connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &OptionDialog::onAccept);

    //project
    connect(ui->pAddButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::addAction);
    connect(ui->pDeleteButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::deleteAction);
    connect(ui->pUpButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::upAction);
    connect(ui->pDownButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::downAction);
    connect(ui->pEditButton, &QPushButton::clicked, ui->profileTableWidget, &ProfileTable::editAction);

    //application variant
    connect(ui->vAddButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::addAction);
    connect(ui->vCopyButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::copyAction);
    connect(ui->vDeleteButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::deleteAction);
    connect(ui->vUpButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::upAction);
    connect(ui->vDownButton, &QPushButton::clicked, ui->variantTableWidget, &StringTable::downAction);

    //search variant
    connect(ui->sAddButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::addAction);
    connect(ui->sCopyButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::copyAction);
    connect(ui->sDeleteButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::deleteAction);
    connect(ui->sDownButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::upAction);
    connect(ui->sUpButton, &QPushButton::clicked, ui->searchTableWidget, &SearchTable::downAction);

    //extend variant
    connect(ui->eAddButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::addAction);
    connect(ui->eDeleteButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::deleteAction);
    connect(ui->eUpButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::upAction);
    connect(ui->eDownButton, &QPushButton::clicked, ui->pluginsTreeWidget, &PluginsTree::downAction);

    //langcombobox
    initLanguageSelection();
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setupItem(){
    ui->listWidget->addItem(tr("General"));
    ui->listWidget->addItem(tr("Profile"));
    ui->listWidget->addItem(tr("Global Variant"));
    ui->listWidget->addItem(tr("Search"));
    ui->listWidget->addItem(tr("Plugins"));

    ui->listWidget->item(0)->setSelected(true);
    ui->listWidget->setIconSize(QSize(32,32));

    ui->listWidget->item(0)->setIcon(QIcon(":/default_icons/settings.png"));
    ui->listWidget->item(1)->setIcon(QIcon(":/default_icons/others.png"));
    ui->listWidget->item(2)->setIcon(QIcon(":/default_icons/string.png"));
    ui->listWidget->item(3)->setIcon(QIcon(":/default_icons/search.png"));
    ui->listWidget->item(4)->setIcon(QIcon(":/default_icons/extras.png"));
}

void OptionDialog::setSettings()
{
    QSettings settings( sc.OUTPUT_FILE_ABR, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABR);
    settings.setValue(sc.ABR_THEME, ui->themeComboBox->currentText());
    settings.setValue(sc.ABR_FONT, ui->windowFontComboBox->currentText());
    settings.setValue(sc.ABR_FONTSIZE, ui->windowFontSizeSpinBox->value());
    storeLanguageSelection(&settings);
    settings.endGroup();
}

void OptionDialog::loadSettings()
{
    QSettings settings( sc.OUTPUT_FILE_ABR, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABR);
    ui->themeComboBox->setCurrentText(settings.value(sc.ABR_THEME, "Default").toString());
    ui->windowFontComboBox->setCurrentFont(QFont(settings.value(sc.ABR_FONT, QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(settings.value(sc.ABR_FONTSIZE, QApplication::font().pointSize()).toInt());
    loadLanguageSelection(&settings);
    settings.endGroup();
}

void OptionDialog::initLanguageSelection()
{
#ifdef QT_DEBUG
    QDirIterator dit("../../src/translation", QStringList() << "*.qm", QDir::Files);
#else
    QDirIterator dit("translation", QStringList() << "*.qm", QDir::Files);
#endif
    while (dit.hasNext()){
        QFileInfo file(dit.next());
        QString strloc = file.baseName().split("_").last();
        QLocale locale(strloc);
        ui->languageComboBox->addItem(QLocale::languageToString(locale.language()));
    }
}

void OptionDialog::storeLanguageSelection(QSettings *setting)
{
    QString selected = ui->languageComboBox->currentText();

    QList<QLocale> locales = QLocale::matchingLocales(QLocale::AnyLanguage,
                             QLocale::AnyScript,
                             QLocale::AnyCountry);

    for(const QLocale& loc : locales){
        if(QLocale::languageToString(loc.language()) == selected){
            setting->setValue(sc.ABR_LANGUAGE, loc.bcp47Name());
            break;
        }
    }
}

void OptionDialog::loadLanguageSelection(QSettings *setting)
{
    QLocale defloc;
    QLocale locale(setting->value(sc.ABR_LANGUAGE, defloc.bcp47Name()).toString());
    ui->languageComboBox->setCurrentText(QLocale::languageToString(locale.language()));
}

//change list to stackedwidget
void OptionDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void OptionDialog::onAccept()
{
    setSettings();
}

void OptionDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    loadSettings();

    // Qss is not reflected in the displayed dialog
    ui->searchTableWidget->setStyleSheet(this->styleSheet());
}

void OptionDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    this->hide();
}
