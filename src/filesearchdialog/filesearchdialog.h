/*
 * Copyright 2016-2021 karakirimu
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

#ifndef FILESEARCHDIALOG_H
#define FILESEARCHDIALOG_H

#include "filesearchdialog_global.h"

#include "../filesearchloader/filesearchloader.h"
#include "../variantconverter/variantconverter.h"
#include "../searchxmlbuilder/searchxmlbuilder.h"
#include <QDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>

#define KB 1024
#define MB 1048576
#define GB 1073741824
#define TB 1099511627776

namespace Ui {
class FileSearchDialog;
}

class FileSearchDialog : public QDialog
{
    Q_OBJECT
public:
    FILESEARCHDIALOGSHARED_EXPORT explicit FileSearchDialog(QWidget *parent = nullptr);
    FILESEARCHDIALOGSHARED_EXPORT ~FileSearchDialog();

    FILESEARCHDIALOGSHARED_EXPORT void loadSettingList(int index, const QList<QStringList> *data);
protected slots:
    void onAccept();
    void onReject();
    void openAction();
    void searchAction();
    void dirEditFinished(QString text);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::FileSearchDialog *ui;
    SearchXmlBuilder *builder;
    bool editflag;
    int editindex;

    void createList(QList<QStringList> *newlist);
    QString timeToSeconds(QString data);
    QString secondsToTime(QString data);
    QString encodeToBytes(QString bytes, int comboidx);
    QString decodeFromBytes(QString bytes, int comboidx);
    void setFsizeComboBox(int num, int comboidx);
};

#endif // FILESEARCHDIALOG_H
