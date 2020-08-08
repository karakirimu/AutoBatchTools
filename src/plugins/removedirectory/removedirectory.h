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

#ifndef REMOVEDIRECTORY_H
#define REMOVEDIRECTORY_H

#include "removedirectory_global.h"
#include "removedirectorywidget.h"
#include "../plugininterface/extraplugininterface.h"
#include <QFile>
#include <QDir>
#include <QPoint>
#include <QFileInfo>
//#include <removedirectorydialog.h>
#include <QDirIterator>

class REMOVEDIRECTORYSHARED_EXPORT RemoveDirectory
        : public ExtraPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "abr.ExtraPluginInterface.RemoveDirectory" FILE "removedirectory.json")
    Q_INTERFACES(ExtraPluginInterface)

public:
    RemoveDirectory();
    ~RemoveDirectory();

    int functionMain(int argc, QStringList *args);

private:
    RemoveDirectoryWidget *swidget;

};

#endif // REMOVEDIRECTORY_H
