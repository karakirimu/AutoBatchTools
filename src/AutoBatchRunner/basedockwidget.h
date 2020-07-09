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

#ifndef BASEDOCKWIDGET_H
#define BASEDOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QEvent>
#include <QDebug>

#define Y_RANGE 12

class BaseDockWidget : public QDockWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Autohide MEMBER autohide NOTIFY autohideOptionChanged)

public:
    explicit BaseDockWidget(QWidget *parent = nullptr);
    ~BaseDockWidget();

    bool isAutohide() const;

public slots:
    void setAutohide(bool value);

signals:
    void autohideOptionChanged();

private slots:
    void showTitleBar();

private:
    bool eventFilter(QObject *watched, QEvent *event);

    //setting data
    bool autohide;

    //widget data
    QWidget *def;
    QWidget *empty;
};

#endif // BASEDOCKWIDGET_H
