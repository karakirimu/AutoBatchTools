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

#include "basedockwidget.h"

BaseDockWidget::BaseDockWidget(QWidget *parent)
    : QDockWidget(parent), autohide(false)
{

    installEventFilter(this);
    def = titleBarWidget();

    empty = new QWidget;
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

    if(autohide && event->type() == QEvent::ToolTip){
        if(widget()->mapFromGlobal(QCursor::pos()).y() <= Y_RANGE){
            setTitleBarWidget(def);
        }
    }

    return QObject::eventFilter(watched, event);
}

bool BaseDockWidget::isAutohide() const
{
    return autohide;
}

void BaseDockWidget::setAutohide(bool value)
{
    if(autohide == value) return;

    autohide = value;

    if(!this->isFloating() && autohide){
        setTitleBarWidget(empty);
    }else{
        setTitleBarWidget(def);
    }
}

void BaseDockWidget::showTitleBar()
{
    this->setTitleBarWidget(def);
}
