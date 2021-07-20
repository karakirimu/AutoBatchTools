/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "autobatchrunner.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
    constexpr const char *TR_PATH = "../src/translation/abt_";
    constexpr const char *PL_TR_PATH = "../src/translation/plugin";
#else
    constexpr const char *TR_PATH = "translation/abt_";
    constexpr const char *PL_TR_PATH = "translation/plugin";
#endif

    QApplication a(argc, argv);

    QSettings settings( SettingConstant().OUTPUT_FILE_ABR, QSettings::IniFormat );
    settings.beginGroup(SettingConstant().GROUP_ABR);
    QLocale locale;
    QString lang
        = settings.value(SettingConstant().ABR_LANGUAGE
                         , locale.bcp47Name()).toString();
    settings.endGroup();

    // Load tranlation
    QTranslator translator;
    if(!translator.load(TR_PATH + lang)){
        qDebug() << "Translation file load failed. "
                 << " Path : " << QDir::currentPath() << lang;
    }
    a.installTranslator(&translator);

    // Load plugin translation
    QDirIterator dit(PL_TR_PATH
                     , QStringList() << ("*_" + lang + ".qm")
                     , QDir::Files);

    QList<QTranslator *> translatorlist;

    while (dit.hasNext()){
        QTranslator *trans = new QTranslator();
        QString path = dit.next();
        if(trans->load(path)){
            a.installTranslator(trans);
            translatorlist.append(trans);
        }
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
