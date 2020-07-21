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

#ifndef MAINSCHEDULER_H
#define MAINSCHEDULER_H

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
};

#endif // MAINSCHEDULER_H
