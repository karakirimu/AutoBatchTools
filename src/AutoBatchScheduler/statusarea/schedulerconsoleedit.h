/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SCHEDULERCONSOLEEDIT_H
#define SCHEDULERCONSOLEEDIT_H

#include <QLineEdit>
#include <taskschedulerconnector.h>
#include <QKeyEvent>

class SchedulerConsoleEdit : public QLineEdit
{
public:
    explicit SchedulerConsoleEdit(QWidget *parent = nullptr);
    ~SchedulerConsoleEdit() override;

    void setTaskSchedulerConnector(TaskSchedulerConnector *task);

protected:
    void keyPressEvent(QKeyEvent *keyevent) override;

private slots:
    void sendMessage();

private:
    TaskSchedulerConnector *taskc;
};

#endif // SCHEDULERCONSOLEEDIT_H
