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

#include "takeline.h"

TakeLinePlugin::TakeLinePlugin()
{
    PluginInformation pinfo;
    pinfo.name = "TakeLine";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = tr("Delete a specified line number from file or text.");

    pinfo.hassettingwidget = true;
    tlwidget = new TakeLineWidget();
    pinfo.settingwidget = tlwidget;

    setInformation(pinfo);
}

TakeLinePlugin::~TakeLinePlugin()
{
    delete tlwidget;
}

int TakeLinePlugin::functionMain(int argc, QStringList *args)
{
    if(argc != 3) return -1;

    QFile file(args->at(FILEORSTRING));
    int targetline = static_cast<QString>(args->at(LINENUM)).toInt();
    QString str;

    if(file.exists() && file.open(QFile::ReadOnly)){

        //file mode
        QTextStream in(&file);
        int i = 1;

        while(!in.atEnd()){
            str = in.readLine();
            if(i == targetline) break;
            if(i++ > targetline){
                str = "";
                break;
            }
        }

    }else{
        //string take mode
        QStringList data = static_cast<QString>(args->at(FILEORSTRING)).split('\n');
        if(data.count() < targetline){
            str = "";
        }else{
            targetline -= 1;
            str = data.at(targetline);
        }

    }

    if(str != ""){
        PluginVariant local;
        applicationVariant(&local);
        if(local.localvar->contains(args->at(VAR))) local.localvar->insert(args->at(VAR), str);
        functionMessage(tr("Delete line ") + QString::number(targetline), MessageType::Success);
        return 0;

    }else{
        functionMessage(tr("Can't delete line."), MessageType::Error);
        return -1;
    }
}
