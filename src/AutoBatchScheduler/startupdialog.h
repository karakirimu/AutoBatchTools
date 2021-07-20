/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDateTime>
#include <QDebug>

#include <settingcache/schedulercache.h>
#include <../share/variantconverter.h>

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
