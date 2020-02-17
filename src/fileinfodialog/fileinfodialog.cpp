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
