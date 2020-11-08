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
