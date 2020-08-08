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

#ifndef MAKEDIRECTORY_H
#define MAKEDIRECTORY_H

#include "makedirectory_global.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QDir>
#include <QPoint>
#include <QFileInfo>

class MAKEDIRECTORYSHARED_EXPORT MakeDirectory
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.MakeDirectory" FILE "makedirectory.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    MakeDirectory();
    ~MakeDirectory();

    int functionMain(int argc, QStringList *args);

};

#endif // MAKEDIRECTORY_H
