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

#include "cellinfowidget.h"
#include "statuswidget.h"
#include "ui_statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    installEventFilter(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);

    connect(ui->settingToolButton, &QToolButton::clicked, this, &StatusWidget::settingsClicked);

}

StatusWidget::~StatusWidget()
{ 
    delete ui;
}

void StatusWidget::setTaskSchedulerConnector(TaskSchedulerConnector *task)
{
    taskc = task;
    ui->enabledTableWidget->setTaskSchedulerConnector(task);
}

void StatusWidget::showWidget()
{
    this->show();

    QList<QScreen *> scr = QGuiApplication::screens();
    // exclude taskbar
    QRect rec = scr.at(0)->geometry();

    // include taskbar
    QRect ava = scr.at(0)->availableGeometry();

#ifdef QT_DEBUG
    qDebug() << "height" << rec.height() << ava.height();
    qDebug() << "width" << rec.width() << ava.width();
    qDebug() << "y" << rec.y() << ava.y();
    qDebug() << "x" << rec.x() << ava.x();

#endif

    if(rec.height() > ava.height()){
        int taskbarsize = rec.height() - ava.height();
        if(rec.y() < ava.y()){
            //top
            this->move(rec.width() - this->width() - 30, taskbarsize + 10);

        }else{
            //bottom
            this->move(rec.width() - this->width() - 30, rec.height() - this->height() - taskbarsize - 10);

        }
    }else {

        int taskbarsize = rec.width() - ava.width();

        if(rec.x() < ava.x()){
            //left
            this->move(taskbarsize + 10, rec.height() - this->height() - 10);

        }else{
            //right
            this->move(rec.width() - this->width() - taskbarsize - 10, rec.height() - this->height() - 10);

        }
    }
}

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
