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

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QAbstractButton>
#include <QDialog>
#include <QSettings>
#include <QColorDialog>
#include <QLabel>
#include <QLineEdit>
#include "settingconstant.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    //set listwidget to open
    void setListPos(int num);
    enum{GENERAL,TEST,SEARCH,PLUGINS};

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void onButtonBoxClicked(QAbstractButton *button);
    void openDir();

private:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

    void setupItem();
    void setSettings();
    void loadSettings();

    void initLanguageSelection();
    void storeLanguageSelection(QSettings *setting);
    void loadLanguageSelection(QSettings *setting);

    void colorSelect(QLineEdit *editor);
    void setColorLabel(QLabel *label, QString hexcolor);
    void updateColor(QLineEdit *edit, QLabel *label);

    const QString REGEXP_HEX = "#([0-9a-fA-F]){6}";

    Ui::SettingDialog *ui;
    SettingConstant sc;
};

#endif // SETTINGDIALOG_H
