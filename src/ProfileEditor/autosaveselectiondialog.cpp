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

#include "autosaveselectiondialog.h"
#include "ui_autosaveselectiondialog.h"

AutoSaveSelectionDialog::AutoSaveSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoSaveSelectionDialog)
{
    setWindowFlags(Qt::WindowCloseButtonHint);

    ui->setupUi(this);

    QIcon infoicon = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
    ui->icon->setPixmap(infoicon.pixmap(QSize(32,32)));

    //init table size
    ui->fileListTableWidget->setColumnCount(1);
    ui->fileListTableWidget->setRowCount(0);

    //set header label
    ui->fileListTableWidget \
            ->setHorizontalHeaderLabels((QStringList() << tr("Last Modified")));

    //adjust column
    ui->fileListTableWidget->horizontalHeader() \
            ->setSectionResizeMode(0, QHeaderView::Stretch);

    //set header style
    QssPropertyConstant qpc;
    ui->fileListTableWidget->verticalHeader() \
        ->setProperty(qpc.VERTICAL_HEADER_STYLE, qpc.VERTICAL_HEADER_ENABLE);

}

AutoSaveSelectionDialog::~AutoSaveSelectionDialog()
{
    delete ui;
}

void AutoSaveSelectionDialog::setAutoSaveFileList(QStringList *list)
{
    ui->fileListTableWidget->setRowCount(list->count());
    filelist = list;

    int i = 0;
    for(QString path : *list){
        QFileInfo info(path);
        QString date = info.lastRead().toString("yyyy/MM/dd HH:mm:ss");
        ui->fileListTableWidget \
                ->setItem(i,0,new QTableWidgetItem(date));
        i++;
    }

    ui->fileListTableWidget->selectRow(0);
}

QString AutoSaveSelectionDialog::execDialog()
{
    if(!exec()) return "";

    int current = ui->fileListTableWidget->currentRow();
    return filelist->at(current);
}
