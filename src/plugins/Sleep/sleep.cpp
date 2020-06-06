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

#include "sleep.h"

Sleep::Sleep()
{
    PluginInformation pinfo;
    pinfo.name = "Sleep";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "";
    pinfo.hassettingwidget = true;

    //Create new widget
    swidget = new SleepWidget();
    pinfo.settingwidget = swidget;

    setInformation(pinfo);
}

Sleep::~Sleep()
{
    delete swidget;
}

int Sleep::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        functionMessage(tr("Argument is missing"), MessageType::Error);
        return -1;
    }

    int time = (args->at(0)).toInt();

    QEventLoop loop;
    QTimer::singleShot( time, &loop, SLOT(quit()));
    loop.exec();

    return 0;
}

//int Sleep::launchSettingWidget(QStringList *currentargs, QStringList *resultargs, QPoint parentpos, QString parentstylesheet)
//{
//    SleepDialog *tdialog = new SleepDialog(currentargs);
//    tdialog->move(parentpos - tdialog->rect().center());
//    tdialog->setStyleSheet(parentstylesheet);

//    if(tdialog->exec() == QDialog::Accepted){
//        *resultargs = tdialog->getargs();
//    }

//    delete tdialog;
//    return 0;
//}
