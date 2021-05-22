/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "fileinfodialog.h"
#include "ui_fileinfodialog.h"

#include <QFileIconProvider>

FileInfoDialog::FileInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileInfoDialog)
{
    // delete ui on dialog closed
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->setupUi(this);
}

FileInfoDialog::~FileInfoDialog()
{
    delete ui;
}

void FileInfoDialog::setFileInfo(QString file)
{
    QFileInfo info(file);

    if(info.isFile()){
        setWindowIcon(this->icon());
        ui->filename->setText(info.fileName());
        ui->filepath->setPlainText(info.canonicalFilePath());
        ui->extension->setText(info.completeSuffix());
        ui->filesize->setText(QString::number(info.size()).append(" Bytes"));
        ui->created->setText(info.birthTime().toString("yyyy/MM/dd hh:mm:ss"));
        ui->updated->setText(info.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
    }
}

QIcon FileInfoDialog::icon(){
    QFileIconProvider *provider = new QFileIconProvider();
    QIcon icon = provider->icon(provider->File);
    delete provider;
    return icon;
}

void FileInfoDialog::on_buttonBox_accepted()
{
    accept();
}
