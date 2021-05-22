/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
    ui->fileListTableWidget->setRowCount(static_cast<int>(list->count()));
    filelist = list;

    int i = 0;
    for(const QString& path : *list){
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
