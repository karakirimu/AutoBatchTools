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

#include "regexpinputrename.h"

RegExpInputRename::RegExpInputRename()
{
    PluginInformation pinfo;
    pinfo.name = "RegExpInputRename";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "When converting with multiple regular expressions, \n"
                    "the regular expression and the replacement character \n"
                    "are divided according to the characters specified in \n"
                    "the next text box, and character conversion is performed \n"
                    "from the forward.";

    pinfo.hassettingwidget = true;

    rewidget = new RegExpInputRenameWidget();
    pinfo.settingwidget = rewidget;

    setInformation(pinfo);
}

RegExpInputRename::~RegExpInputRename()
{
    delete rewidget;
}

int RegExpInputRename::functionMain(int argc, QStringList *args)
{
    if(argc != 4) return -1;

    // argument 1 is file or dir
    QFileInfo finfo(args->at(Input));

    // get file or basename
    QString base = finfo.baseName();

    // split argument 3 and 4


    // filename or basename replaces at 3 to 4

    // rename file or dir

    return 0;
}

//int RegExpInputRename::launchSettingWidget(QStringList *currentargs, QStringList *resultargs, QPoint parentpos, QString parentstylesheet)
//{
//    RegExpInputRenameDialog *tdialog = new RegExpInputRenameDialog(currentargs);
//    tdialog->move(parentpos - tdialog->rect().center());
//    tdialog->setStyleSheet(parentstylesheet);

//    if(tdialog->exec() == QDialog::Accepted){
//        *resultargs = tdialog->getargs();
//    }

//    delete tdialog;
//    return 0;
//}
