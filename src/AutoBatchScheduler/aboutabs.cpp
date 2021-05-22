/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "aboutabs.h"
#include "ui_aboutabs.h"

AboutABS::AboutABS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutABS)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    QIcon icons(":/app_icons/app_abs.ico");
    ui->icon->setPixmap(icons.pixmap(128,128));
    ui->version->setText(QString("Version %1").arg(CURRENT_VERSION));

}

AboutABS::~AboutABS()
{
    delete ui;
}
