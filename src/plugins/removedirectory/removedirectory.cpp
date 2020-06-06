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

#include "removedirectory.h"

RemoveDirectory::RemoveDirectory()
{
    PluginInformation pinfo;
    pinfo.name = "RemoveDirectory";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "argument 1 : The folder path you want to remove\n"
                    "argument 2 : Whether to delete files in folder (-rf)";
    pinfo.hassettingwidget = true;

    //Create new widget
    swidget = new RemoveDirectoryWidget();
    pinfo.settingwidget = swidget;

    setInformation(pinfo);
}

RemoveDirectory::~RemoveDirectory()
{
    delete swidget;
}

int RemoveDirectory::functionMain(int argc, QStringList *args)
{
    if(argc < 1){
        functionMessage(tr("Argument is missing"), MessageType::Error);
        return -1;
    }

    bool dirbool;

    QFileInfo info(args->at(0));

    if(!(info.exists() && info.isDir())){
        functionMessage(tr("Input folder does not exist."), MessageType::Error);
        return -1;
    }

    QDir dir(info.absoluteFilePath());

    if(argc == 2 && args->at(1) == "-rf"){
        QDirIterator dit(dir.path(), QStringList() << "*", QDir::Files | QDir::NoSymLinks);

        while (dit.hasNext()){
            QFile::remove(dit.next());
        }
    }

    functionMessage("Input : " + info.absoluteFilePath(), MessageType::Update);

    QDir dir2;

    dirbool = dir2.rmdir(info.absoluteFilePath());

    return dirbool ? 0 : -1;
}

//int RemoveDirectory::launchSettingWidget(QStringList *currentargs, QStringList *resultargs \
//                                         , QPoint parentpos, QString parentstylesheet)
//{
//    RemoveDirectoryDialog *tdialog = new RemoveDirectoryDialog(currentargs);
//    tdialog->move(parentpos - tdialog->rect().center());
//    tdialog->setStyleSheet(parentstylesheet);

//    if(tdialog->exec() == QDialog::Accepted){
//        *resultargs = tdialog->getargs();
//    }

//    delete tdialog;
//    return 0;
//}
