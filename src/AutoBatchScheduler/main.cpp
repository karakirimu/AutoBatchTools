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
#include <QLocale>
#include <QTranslator>
#include "settingconstant.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set default text codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    SettingConstant sc;
    QSettings settings( sc.OUTPUT_FILE_ABS, QSettings::IniFormat );
    settings.beginGroup(sc.GROUP_ABS);

    QLocale locale;
    QString lang = settings.value(sc.ABS_LANGUAGE, locale.bcp47Name()).toString();
    bool minimizewindow = settings.value(sc.ABS_MINIMIZE_WINDOW, false).toBool();
    settings.endGroup();

    QTranslator translator;
#ifdef QT_DEBUG

    bool success = false;
    success = translator.load("../../src/translation/abt_" + lang);

    qDebug() << "load : " << success << " Path : " << QDir::currentPath() << lang;

#else
    translator.load("translation/abt_" + lang);
#endif
    a.installTranslator(&translator);

    MainScheduler w;
    if(!minimizewindow){
        w.show();
    }

    return a.exec();
}
