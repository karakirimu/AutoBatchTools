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

}

AboutABR::~AboutABR()
{
    delete ui;
}