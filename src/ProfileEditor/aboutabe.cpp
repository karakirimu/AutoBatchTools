/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "aboutabe.h"
#include "ui_aboutabe.h"

AboutABE::AboutABE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutABE)
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->icon->setPixmap(QIcon(":/app_icons/app_pe.ico").pixmap(128,128));
    ui->version->setText(QString("Version %1").arg(CURRENT_VERSION));
}

AboutABE::~AboutABE()
{
    delete ui;
}
