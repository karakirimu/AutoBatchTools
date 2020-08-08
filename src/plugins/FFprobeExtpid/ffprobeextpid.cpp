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

#include "ffprobeextpid.h"

FFprobeExtpid::FFprobeExtpid()
{
    PluginInformation pinfo;
    pinfo.name = "FFprobeExtpid";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "";
    pinfo.hassettingwidget = true;

    fswidget = new FFprobeExtPidWidget();
    pinfo.settingwidget = fswidget;

    setInformation(pinfo);
}

FFprobeExtpid::~FFprobeExtpid()
{
    delete fswidget;
}

int FFprobeExtpid::functionMain(int argc, QStringList *args)
{
    if(argc < 3){
        functionMessage(tr("The argument is missing."), MessageType::Error);
        return -1;
    }

    QString proberesult = args->at(0);
    QFile res(proberesult);

    if(!res.open(QIODevice::Text | QIODevice::ReadOnly)) return -1;

    QString videopid = "";
    QString audiopid = "";
    QString tmparr;

    QRegExp re = QRegExp("(\\[0x[0-9][0-9].\\])");

    QTextStream in(&res);

    while(!in.atEnd()){
        tmparr = in.readLine();

        if(tmparr.contains("Video: mpeg2video")){
            re.indexIn(tmparr);

            // [0x...]
            tmparr = re.capturedTexts().at(0);

            // 0x...
            videopid.append(tmparr.mid(1,5));
            videopid.append("\n");
        }

        if(tmparr.contains("Audio: aac")){
            re.indexIn(tmparr);
            // [0x...]
            tmparr = re.capturedTexts().at(0);

            // 0x...
            audiopid.append(tmparr.mid(1,5));
            audiopid.append("\n");
        }
    }

    res.close();

    writeToText(args->at(1), videopid);
    writeToText(args->at(2), audiopid);
    return 0;
}

bool FFprobeExtpid::writeToText(QString filename, QString text)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}
