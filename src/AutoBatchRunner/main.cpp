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

#include "autobatchrunner.h"
#include <QApplication>
#include <QLocale>
//#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    constexpr const char *TR_PATH = "../../src/translation/abt_";
    constexpr const char *PL_TR_PATH = "../../src/translation/plugin";
#else
    constexpr const char *TR_PATH = "translation/abt_";
    constexpr const char *PL_TR_PATH = "translation/plugin";
#endif

    QApplication a(argc, argv);

    // set default text codec
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QSettings settings( SettingConstant().OUTPUT_FILE_ABR, QSettings::IniFormat );
    settings.beginGroup(SettingConstant().GROUP_ABR);
    QLocale locale;
    QString lang = settings.value(SettingConstant().ABR_LANGUAGE, locale.bcp47Name()).toString();
    settings.endGroup();

    // Load tranlation
    QTranslator translator;
    if(!translator.load(TR_PATH + lang)){
        qDebug() << "Translation file load failed. " << " Path : " << QDir::currentPath() << lang;
    }
    a.installTranslator(&translator);

    // Load plugin translation
    QDirIterator dit(PL_TR_PATH, QStringList() << ("*_" + lang + ".qm"), QDir::Files);
    QList<QTranslator *> translatorlist;
    while (dit.hasNext()){
        QTranslator *trans = new QTranslator();
        QString path = dit.next();
        trans->load(path);
        a.installTranslator(trans);
        translatorlist.append(trans);
    }

    AutoBatchRunner w;
    w.show();

    // Execution
    int result = a.exec();

    // Delete translator instance
    for(QTranslator *t : translatorlist){
        delete t;
    }

    return result;
}
