/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QSettings>
#include <settingconstant.h>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog() override;

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void onAccept();

private:
    Ui::OptionDialog *ui;

    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void setupItem();
    void setSettings();
    void loadSettings();

    void initLanguageSelection();
    void storeLanguageSelection(QSettings *setting);
    void loadLanguageSelection(QSettings *setting);

    SettingConstant sc;
};

#endif // OPTIONDIALOG_H
