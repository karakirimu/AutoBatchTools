/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "basedockwidget.h"

BaseDockWidget::BaseDockWidget(QWidget *parent)
    : QDockWidget(parent), autohide(false)
{
    installEventFilter(this);
    def = titleBarWidget();
    empty = new QWidget();
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    connect(this, &BaseDockWidget::topLevelChanged, this, &BaseDockWidget::showTitleBar);

}

BaseDockWidget::~BaseDockWidget()
{
    delete empty;
}

bool BaseDockWidget::eventFilter(QObject *watched, QEvent *event)
{

    if(!isFloating() && autohide && event->type() == QEvent::Leave){
        setTitleBarWidget(empty);
    }
    if(autohide && event->type() == QEvent::ToolTip
            && widget()->mapFromGlobal(QCursor::pos()).y() <= Y_RANGE){
        setTitleBarWidget(def);
    }

    return QObject::eventFilter(watched, event);
}

bool BaseDockWidget::isAutohide() const
{
    return autohide;
}

void BaseDockWidget::setAutohide(bool value)
{
    if(autohide == value){
        return;
    }else{
        autohide = value;

        if(!this->isFloating() && autohide){
            setTitleBarWidget(empty);
        }else{
            setTitleBarWidget(def);
        }
    }
}

void BaseDockWidget::showTitleBar()
{
    this->setTitleBarWidget(def);
}
