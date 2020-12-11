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
