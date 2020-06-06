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

#ifndef TAKELINEPLUGIN_H
#define TAKELINEPLUGIN_H

#include "takeline_global.h"
#include "takelinewidget.h"
#include "../plugininterface/extraplugininterface.h"
//#include "takelinedialog.h"

#include <QFile>
#include <QTextStream>

class TAKELINEPLUGINSHARED_EXPORT TakeLinePlugin
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.TakeLinePlugin" FILE "takelineplugin.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    TakeLinePlugin();
    ~TakeLinePlugin();

    int functionMain(int argc, QStringList *args);

//    QString tooltipString(){
//        return tr("This function can take line 'n'.\n"
//                  "arguments 1 : filename or string with '\\n'\n"
//                  "arguments 2 : line number(it starts 0)\n"
//                  "arguments 3 : return local variant name");
//    }

//    QString version(){ return "v 1.0.0"; }
//    QString vendor(){ return "karakirimu"; }

//    bool existsSettingWidget(){return true;}

//    int launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
//                            , QPoint parentpos, QString parentstylesheet);

private:
    enum {FILEORSTRING, LINENUM, VAR};
    TakeLineWidget *tlwidget;
};

#endif // TAKELINEPLUGIN_H
