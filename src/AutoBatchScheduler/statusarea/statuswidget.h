/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QMouseEvent>
#include <QWidget>
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
