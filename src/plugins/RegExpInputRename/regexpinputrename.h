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

#ifndef REGEXPINPUTRENAME_H
#define REGEXPINPUTRENAME_H

#include "regexpinputrename_global.h"
#include "regexpinputrenamewidget.h"
#include "../plugininterface/extraplugininterface.h"
//#include <regexpinputrenamedialog.h>
#include <QFileInfo>
#include <QDir>
#include <QFile>

class REGEXPINPUTRENAMESHARED_EXPORT RegExpInputRename
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.RegExpInputRename" FILE "RegExpInputRename.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    RegExpInputRename();
    ~RegExpInputRename();

     int functionMain(int argc, QStringList *args);

//     int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
//                             , QPoint parentpos, QString parentstylesheet);
 private:
     RegExpInputRenameWidget *rewidget;

     enum RegExpInputRenameList{
         Input,
         RegExp,
         Replace,
         Separator
     };
};

#endif // REGEXPINPUTRENAME_H
