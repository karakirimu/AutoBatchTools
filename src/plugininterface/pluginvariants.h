/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
