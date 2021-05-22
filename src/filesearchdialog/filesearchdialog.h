/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
