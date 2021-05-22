/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "aboutabr.h"
#include "ui_aboutabr.h"

AboutABR::AboutABR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutABR)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    QIcon icons(":/app_icons/app_abr.ico");
    ui->icon->setPixmap(icons.pixmap(128,128));
    ui->version->setText(QString("Version %1").arg(CURRENT_VERSION));

}

AboutABR::~AboutABR()
{
    delete ui;
}
