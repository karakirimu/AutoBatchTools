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

#include <startupxmlbuilder.h>
#include <../variantconverter/variantconverter.h>

#include <QDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>
#include <QRandomGenerator>

namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartupDialog(QWidget *parent = nullptr);
    ~StartupDialog();

    void loadSettingList(int index, const QList<QStringList> *data);
protected slots:
    void onAccept();
    void onReject();
    void everyDaySelected(bool checked);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartupDialog *ui;
    StartupXmlBuilder *builder;
    bool editflag;
    int editindex;
    QString uniquecode;

    void createList(QList<QStringList> *newlist);

    QString daySelectToString();
    QString getRandomString(int length);
    QString timeToSeconds(QString data);
    QString secondsToTime(QString data);
    void StringToDaySelect(QString data);
};

#endif // STARTUPDIALOG_H
