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

#ifndef AUTOSAVESELECTIONDIALOG_H
#define AUTOSAVESELECTIONDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QDateTime>

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
