/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
            this->move(rec.width() - this->width() - 10, taskbarsize);

        }else{
            //bottom
            this->move(rec.width() - this->width() - 10, rec.height() - this->height() - taskbarsize);

        }
    }else {

        int taskbarsize = rec.width() - ava.width();

        if(rec.x() < ava.x()){
            //left
            this->move(taskbarsize, rec.height() - this->height() - 10);

        }else{
            //right
            this->move(rec.width() - this->width() - taskbarsize, rec.height() - this->height() - 10);

        }
    }
}

void StatusWidget::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
