/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef AUTOSAVESELECTIONDIALOG_H
#define AUTOSAVESELECTIONDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QDateTime>
#include <../share/qss/qsspropertyconstant.h>

namespace Ui {
class AutoSaveSelectionDialog;
}

class AutoSaveSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoSaveSelectionDialog(QWidget *parent = nullptr);
    ~AutoSaveSelectionDialog();

    void setAutoSaveFileList(QStringList *list);
    QString execDialog();

private:
    Ui::AutoSaveSelectionDialog *ui;
    QStringList *filelist;
};

#endif // AUTOSAVESELECTIONDIALOG_H
