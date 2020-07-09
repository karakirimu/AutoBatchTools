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

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QMouseEvent>
#include <QWidget>
#include <QDesktopWidget>
#include <QItemDelegate>
#include <QScreen>
#include <taskschedulerconnector.h>

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QWidget *parent = nullptr);
    ~StatusWidget();

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);
    void showWidget();
signals:
    void launchSettings();

public slots:
    void settingsClicked(){emit launchSettings();}

private:
    void closeEvent(QCloseEvent *event);

    Ui::StatusWidget *ui;
    TaskSchedulerConnector *taskc;
};

#endif // STATUSWIDGET_H
