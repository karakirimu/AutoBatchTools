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

#include "mainscheduler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set default text codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainScheduler w;

    QSettings settings( "./settings.ini", QSettings::IniFormat );
    settings.beginGroup("scheduler_startup");
    if(!settings.value("HIDEWINDOW", false).toBool()){
        w.show();
    }
    settings.endGroup();
    return a.exec();
}
