/*
 * Copyright 2016-2021 karakirimu
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

#ifndef PLUGINVARIANTS_H
#define PLUGINVARIANTS_H

#include "pluginwidget.h"
#include <QString>
#include <QStringList>
#include <QHash>

class PluginInformation {
public:
    //! plugin name
    QString name;

    //! functions version (preferred: v X.XX)
    QString version;

    //! author name
    QString author;

    //! explain string
    QString tooltip;

    //! true if you use setting widget.
    bool hassettingwidget;

    //! Widget for displaying settings.
    //! Not used if issettingwidget is false.
    PluginWidget *settingwidget;
};

class PluginVariant {
public:
    PluginVariant() { filevar = new QStringList(); }
    ~PluginVariant() { delete filevar; }

    //! local variable
    QHash<QString, QString> *localvar;

    //! global variable
    QHash<QString, QString> globalvar;

    //! input file variable
    QStringList *filevar;
};

enum class MessageType {
    //! 0
    Update,
    //! 1
    Success,
    //! 2
    Error
};

enum class InputType {
    //! value: 0 type: QHash<QString, QString>
    Local,
    //! value: 1 type: QHash<QString, QString>
    Global,
    //! value: 2 type: QStringList
    File
};

#endif // PLUGINVARIANTS_H
