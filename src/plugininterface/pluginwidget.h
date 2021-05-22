/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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

#endif // PLUGINWIDGET_H
