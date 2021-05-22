/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef MAINSCHEDULER_H
#define MAINSCHEDULER_H

#include "settingconstant.h"
#include "taskschedulerconnector.h"

#include <QMainWindow>
#include <QMouseEvent>
#include <QSettings>
#include <systemtray.h>
#include <aboutabs.h>

namespace Ui {
class MainScheduler;
}

class MainScheduler : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScheduler(QWidget *parent = nullptr);
    ~MainScheduler();

private slots:
    void onSchedulerButtonClicked();
    void onOptionButtonClicked();
    void showWindow();

    void closeEvent(QCloseEvent *event);

private:
    void saveSettings();
    void restoreSettings();
    void themeChangeAction();
    void about();

    void initLanguageSelection();
    void storeLanguageSelection(QSettings *setting);
    void loadLanguageSelection(QSettings *setting);

    Ui::MainScheduler *ui;
    SystemTray *sysTray;
    TaskSchedulerConnector *taskscheduler;
    SettingConstant sc;
};

#endif // MAINSCHEDULER_H
