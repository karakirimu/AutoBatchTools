/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
