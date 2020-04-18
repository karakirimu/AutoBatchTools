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

#ifndef PLUGINWIDGET_H
#define PLUGINWIDGET_H

#include <QtWidgets/QWidget>

class PluginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PluginWidget(QWidget *parent = nullptr):QWidget(parent){}
    virtual ~PluginWidget(){}

signals:
    void sendList(QStringList);

public slots:
    virtual void receiveList(QStringList args) = 0;

private:

};

//Q_DECLARE_INTERFACE(PluginWidget, "abr.PluginWidget/1.0")

#endif // PLUGINWIDGET_H
