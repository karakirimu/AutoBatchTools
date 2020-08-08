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

#ifndef BUFFEREDCOPY_H
#define BUFFEREDCOPY_H

#include "bufferedcopy_global.h"
#include "../plugininterface/extraplugininterface.h"
#include "settingwidget.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCryptographicHash>

class BUFFEREDCOPYSHARED_EXPORT BufferedCopy
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.BufferedCopy" FILE "bufferedcopy.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    BufferedCopy();
    ~BufferedCopy();

    int functionMain(int argc, QStringList *args);

private:
    bool copyData(QString source, QString dest, long cachesize);
    bool checkHash(QString source, QString dest);

    QByteArray sha1Hash(QString file);

    SettingWidget* swidget;
};

#endif // BUFFEREDCOPY_H
