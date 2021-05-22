/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef CELLINFOWIDGET_H
#define CELLINFOWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <taskschedulerconnector.h>

namespace Ui {
class CellInfoWidget;
}

class CellInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CellInfoWidget(QWidget *parent = nullptr);
    ~CellInfoWidget();

    int indicateHeight();

    //set latter of setobjectname
    void setConsoleTarget(TaskSchedulerConnector *taskc);

    //confirm console status
    bool isConsoleVisible();
    void setConsoleVisible(bool show);

    QHash<int, QVariant> getState() const;
    void restoreState(const QHash<int, QVariant> &state);

signals:
    //send own objectname
    void consoleButtonClicked(QString);
    void pauseButtonClicked(QString);
    void stopButtonClicked(QString);
    void changeRunStatus(bool);

public slots:
    //init set
    void setProgressminmax(int start, int end);
    void setProfileName(QString name);

    //call when runprocess start or schedule started
    void started();
    void scheduled();

    //update information in runprocess
    void updateProcess(QString data, int type);
    void updateProgress(int num);

    void setTimerEnd(const QString &date);

    void updateErrorProgress(int num);
    void updateErrorText(QString message);

    //toggle enabled or disabled
    void setRunStatus(bool enabled);
    void onRunStatusChange(bool enabled);

private slots:
    void onConsoleButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();

private:

    enum TempSaveTag {
        UpdateText = 0,
        TimerText,
//        Progress,
        ConsoleText,
        ProgressEnable,
        PauseEnable,
        StopEnable,
        ConsoleVisible
    };

    Ui::CellInfoWidget *ui;
};

#endif // CELLINFOWIDGET_H
