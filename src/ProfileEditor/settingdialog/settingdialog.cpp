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

    QRegularExpressionValidator validator(QRegularExpression(REGEXP_HEX), this);

    // color textEdit
    ui->colorInfoLineEdit->setValidator(&validator);
    ui->colorExecLineEdit->setValidator(&validator);
    ui->colorSearchLineEdit->setValidator(&validator);
    ui->colorPluginLineEdit->setValidator(&validator);
    ui->colorProfileLineEdit->setValidator(&validator);

    ui->textColorInfoLineEdit->setValidator(&validator);
    ui->textColorExecLineEdit->setValidator(&validator);
    ui->textColorSearchLineEdit->setValidator(&validator);
    ui->textColorPluginLineEdit->setValidator(&validator);
    ui->textColorProfileLineEdit->setValidator(&validator);

    connect(ui->colorInfoLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->colorInfoLineEdit, ui->colorInfo); });
    connect(ui->colorExecLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->colorExecLineEdit, ui->colorExec); });
    connect(ui->colorSearchLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->colorSearchLineEdit, ui->colorSearch); });
    connect(ui->colorPluginLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->colorPluginLineEdit, ui->colorPlugin); });
    connect(ui->colorProfileLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->colorProfileLineEdit, ui->colorProfile); });

    connect(ui->textColorInfoLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->textColorInfoLineEdit, ui->textColorInfo); });
    connect(ui->textColorExecLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->textColorExecLineEdit, ui->textColorExec); });
    connect(ui->textColorSearchLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->textColorSearchLineEdit, ui->textColorSearch); });
    connect(ui->textColorPluginLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->textColorPluginLineEdit, ui->textColorPlugin); });
    connect(ui->textColorProfileLineEdit, &QLineEdit::textChanged,
            [=]{ updateColor(ui->textColorProfileLineEdit, ui->textColorProfile); });

    // color window
    connect(ui->colorInfoToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->colorInfoLineEdit); });
    connect(ui->colorExecToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->colorExecLineEdit); });
    connect(ui->colorSearchToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->colorSearchLineEdit); });
    connect(ui->colorPluginToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->colorPluginLineEdit); });
    connect(ui->colorProfileToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->colorProfileLineEdit); });

    connect(ui->textColorInfoToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->textColorInfoLineEdit); });
    connect(ui->textColorExecToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->textColorExecLineEdit); });
    connect(ui->textColorSearchToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->textColorSearchLineEdit); });
    connect(ui->textColorPluginToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->textColorPluginLineEdit); });
    connect(ui->textColorProfileToolButton, &QToolButton::clicked,
            [=]{ colorSelect(ui->textColorProfileLineEdit); });


    // revert button
    connect(ui->revertInfoToolButton, &QToolButton::clicked,
            [=]{ ui->colorInfoLineEdit->setText(sc.ABE_INFO_BACKGROUND_DEFAULT); });
    connect(ui->revertExecToolButton, &QToolButton::clicked,
            [=]{ ui->colorExecLineEdit->setText(sc.ABE_EXEC_BACKGROUND_DEFAULT); });
    connect(ui->revertSearchToolButton, &QToolButton::clicked,
            [=]{ ui->colorSearchLineEdit->setText(sc.ABE_SEARCH_BACKGROUND_DEFAULT); });
    connect(ui->revertPluginToolButton, &QToolButton::clicked,
            [=]{ ui->colorPluginLineEdit->setText(sc.ABE_PLUGIN_BACKGROUND_DEFAULT); });
    connect(ui->revertProfileToolButton, &QToolButton::clicked,
            [=]{ ui->colorProfileLineEdit->setText(sc.ABE_PROFILE_BACKGROUND_DEFAULT); });

    connect(ui->textRevertInfoToolButton, &QToolButton::clicked,
            [=]{ ui->textColorInfoLineEdit->setText(sc.ABE_INFO_TEXTCOLOR_DEFAULT); });
    connect(ui->textRevertExecToolButton, &QToolButton::clicked,
            [=]{ ui->textColorExecLineEdit->setText(sc.ABE_EXEC_TEXTCOLOR_DEFAULT); });
    connect(ui->textRevertSearchToolButton, &QToolButton::clicked,
            [=]{ ui->textColorSearchLineEdit->setText(sc.ABE_SEARCH_TEXTCOLOR_DEFAULT); });
    connect(ui->textRevertPluginToolButton, &QToolButton::clicked,
            [=]{ ui->textColorPluginLineEdit->setText(sc.ABE_PLUGIN_TEXTCOLOR_DEFAULT); });
    connect(ui->textRevertProfileToolButton, &QToolButton::clicked,
            [=]{ ui->textColorProfileLineEdit->setText(sc.ABE_PROFILE_TEXTCOLOR_DEFAULT); });

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
    settings.setValue(sc.ABE_PROCESS_HIDE_DESC, ui->showDescCheckBox->isChecked());

    auto getColorOrDefault = [&](QString candidate, QString def){
        QRegularExpression re(REGEXP_HEX);
        QRegularExpressionMatch match = re.match(candidate);
        if(match.hasMatch()){
            return candidate;
        }

        return def;
    };

    settings.setValue(sc.ABE_INFO_BACKGROUND
                      , getColorOrDefault(ui->colorInfoLineEdit->text()
                                            , sc.ABE_INFO_BACKGROUND_DEFAULT));
    settings.setValue(sc.ABE_EXEC_BACKGROUND
                      , getColorOrDefault(ui->colorExecLineEdit->text()
                                            , sc.ABE_EXEC_BACKGROUND_DEFAULT));
    settings.setValue(sc.ABE_SEARCH_BACKGROUND
                      , getColorOrDefault(ui->colorSearchLineEdit->text()
                                            , sc.ABE_SEARCH_BACKGROUND_DEFAULT));
    settings.setValue(sc.ABE_PLUGIN_BACKGROUND
                      , getColorOrDefault(ui->colorPluginLineEdit->text()
                                            , sc.ABE_PLUGIN_BACKGROUND_DEFAULT));
    settings.setValue(sc.ABE_PROFILE_BACKGROUND
                      , getColorOrDefault(ui->colorProfileLineEdit->text()
                                            , sc.ABE_PROFILE_BACKGROUND_DEFAULT));
    settings.setValue(sc.ABE_INFO_TEXTCOLOR
                      , getColorOrDefault(ui->textColorInfoLineEdit->text()
                                            , sc.ABE_INFO_TEXTCOLOR_DEFAULT));
    settings.setValue(sc.ABE_EXEC_TEXTCOLOR
                      , getColorOrDefault(ui->textColorExecLineEdit->text()
                                            , sc.ABE_EXEC_TEXTCOLOR_DEFAULT));
    settings.setValue(sc.ABE_SEARCH_TEXTCOLOR
                      , getColorOrDefault(ui->textColorSearchLineEdit->text()
                                            , sc.ABE_SEARCH_TEXTCOLOR_DEFAULT));
    settings.setValue(sc.ABE_PLUGIN_TEXTCOLOR
                      , getColorOrDefault(ui->textColorPluginLineEdit->text()
                                            , sc.ABE_PLUGIN_TEXTCOLOR_DEFAULT));
    settings.setValue(sc.ABE_PROFILE_TEXTCOLOR
                      , getColorOrDefault(ui->textColorProfileLineEdit->text()
                                            , sc.ABE_PROFILE_TEXTCOLOR_DEFAULT));

    settings.endGroup();
}

void SettingDialog::loadSettings()
{
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    settings.beginGroup(sc.GROUP_ABE);
    ui->tempEdit->setText(settings.value(sc.ABE_AUTOSAVE_DIR, "./").toString());
    ui->autosaveSpinBox->setValue(
        settings.value(sc.ABE_AUTOSAVE_PERIOD, 1).toInt());
    ui->themeComboBox->setCurrentText(
        settings.value(sc.ABE_THEME, "Default").toString());
    ui->windowFontComboBox->setCurrentFont(
        QFont(settings.value(sc.ABE_FONT, QApplication::font().toString()).toString()));
    ui->windowFontSizeSpinBox->setValue(
        settings.value(sc.ABE_FONTSIZE, QApplication::font().pointSize()).toInt());
    loadLanguageSelection(&settings);
    ui->fakeresidentCheckBox->setChecked(
        settings.value(sc.ABE_RUNAS_SCHEDULER, false).toBool());
    ui->showDescCheckBox->setChecked(
        settings.value(sc.ABE_PROCESS_HIDE_DESC, false).toBool());

    QString hexcolor = settings.value(sc.ABE_INFO_TEXTCOLOR
                                      , sc.ABE_INFO_TEXTCOLOR_DEFAULT).toString();
    ui->textColorInfoLineEdit->setText(hexcolor);
    setColorLabel(ui->textColorInfo, hexcolor);

    hexcolor = settings.value(sc.ABE_EXEC_TEXTCOLOR
                              , sc.ABE_EXEC_TEXTCOLOR_DEFAULT).toString();
    ui->textColorExecLineEdit->setText(hexcolor);
    setColorLabel(ui->textColorExec, hexcolor);

    hexcolor = settings.value(sc.ABE_SEARCH_TEXTCOLOR
                              , sc.ABE_SEARCH_TEXTCOLOR_DEFAULT).toString();
    ui->textColorSearchLineEdit->setText(hexcolor);
    setColorLabel(ui->textColorSearch, hexcolor);

    hexcolor = settings.value(sc.ABE_PLUGIN_TEXTCOLOR
                              , sc.ABE_PLUGIN_TEXTCOLOR_DEFAULT).toString();
    ui->textColorPluginLineEdit->setText(hexcolor);
    setColorLabel(ui->textColorPlugin, hexcolor);

    hexcolor = settings.value(sc.ABE_PROFILE_TEXTCOLOR
                              , sc.ABE_PROFILE_TEXTCOLOR_DEFAULT).toString();
    ui->textColorProfileLineEdit->setText(hexcolor);
    setColorLabel(ui->textColorProfile, hexcolor);

    hexcolor = settings.value(sc.ABE_INFO_BACKGROUND
                              , sc.ABE_INFO_BACKGROUND_DEFAULT).toString();
    ui->colorInfoLineEdit->setText(hexcolor);
    setColorLabel(ui->colorInfo, hexcolor);

    hexcolor = settings.value(sc.ABE_EXEC_BACKGROUND
                              , sc.ABE_EXEC_BACKGROUND_DEFAULT).toString();
    ui->colorExecLineEdit->setText(hexcolor);
    setColorLabel(ui->colorExec, hexcolor);

    hexcolor = settings.value(sc.ABE_SEARCH_BACKGROUND
                              , sc.ABE_SEARCH_BACKGROUND_DEFAULT).toString();
    ui->colorSearchLineEdit->setText(hexcolor);
    setColorLabel(ui->colorSearch, hexcolor);

    hexcolor = settings.value(sc.ABE_PLUGIN_BACKGROUND
                              , sc.ABE_PLUGIN_BACKGROUND_DEFAULT).toString();
    ui->colorPluginLineEdit->setText(hexcolor);
    setColorLabel(ui->colorPlugin, hexcolor);

    hexcolor = settings.value(sc.ABE_PROFILE_BACKGROUND
                              , sc.ABE_PROFILE_BACKGROUND_DEFAULT).toString();
    ui->colorProfileLineEdit->setText(hexcolor);
    setColorLabel(ui->colorProfile, hexcolor);
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

void SettingDialog::colorSelect(QLineEdit *editor)
{
    QColor result = QColorDialog::getColor(editor->text(), this );
    if(result.isValid()) {
        qDebug() << "Color : "<< result.name();
    }

    editor->setText(result.name());
}

void SettingDialog::setColorLabel(QLabel *label, QString hexcolor)
{
    QPixmap pixmap(QSize(12,12));
    pixmap.fill(QColor(hexcolor));
    label->setPixmap(pixmap);
}

void SettingDialog::updateColor(QLineEdit *edit, QLabel *label)
{
    QString candidate = edit->text();
    QRegularExpression re(REGEXP_HEX);
    QRegularExpressionMatch match = re.match(candidate);
    if(!match.hasMatch()) return;
    setColorLabel(label, candidate);
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
