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

#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDateTime>
#include <QDebug>

#include <settingcache/schedulercache.h>
#include <../variantconverter/variantconverter.h>

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartupDialog(QWidget *parent = nullptr);
    ~StartupDialog();

    void load(const SchedulerCache &setting);
    const SchedulerCache getSavedSetting() { return settingCache; }

protected slots:
    void onAccept();
    void onReject();
    void everyDaySelected(bool checked);

protected:
    void closeEvent(QCloseEvent *event);

private:
    void updateSetting();

    QString daySelectToString();
    QString timeToSeconds(QString data);
    QString secondsToTime(qint64 num);
    void StringToDaySelect(QString data);

    Ui::StartupDialog *ui;
    SchedulerCache settingCache;
};

#endif // STARTUPDIALOG_H
