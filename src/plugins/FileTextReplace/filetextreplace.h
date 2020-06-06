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

#ifndef FILETEXTREPLACE_H
#define FILETEXTREPLACE_H

#include "filetextreplace_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <filetextreplacewidget.h>
//#include <filetextreplacedialog.h>

class FILETEXTREPLACESHARED_EXPORT FileTextReplace
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.FileTextReplace" FILE "FileTextReplace.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    FileTextReplace();
    ~FileTextReplace();

    int functionMain(int argc, QStringList *args);

//    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
//                            , QPoint parentpos, QString parentstylesheet);

private:
    bool writeToText(QString filename, QString text);

    FileTextReplaceWidget *ftwidget;
};

#endif // FILETEXTREPLACE_H
