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

#include "makedirectory.h"

MakeDirectory::MakeDirectory()
{
    PluginInformation pinfo;
    pinfo.name = "MakeDirectory";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "Create all the directories needed for the folder path.\n"
                    "argument 1 : The folder path you want to create";
    pinfo.hassettingwidget = false;

    setInformation(pinfo);
}

MakeDirectory::~MakeDirectory()
{

}

int MakeDirectory::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        functionMessage(tr("Argument is missing"), MessageType::Error);
        return -1;
    }

    bool dirbool;
    QDir dir;
    dirbool = dir.mkpath(args->at(0));

    return dirbool ? 0 : -1;
}

//int MakeDirectory::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
//                                       , QPoint parentpos, QString parentstylesheet)
//{
//    Q_UNUSED(currentargs)
//    Q_UNUSED(resultargs)
//    Q_UNUSED(parentpos)
//    Q_UNUSED(parentstylesheet)

//    return 0;
//}
