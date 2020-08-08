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

#include "bufferedcopy.h"

BufferedCopy::BufferedCopy(){
    PluginInformation pinfo;
    pinfo.name = "BufferedCopy";
    pinfo.version = "1.00";
    pinfo.author = "karakirimu";
    pinfo.tooltip = "";
    pinfo.hassettingwidget = true;

    //Create new widget
    swidget = new SettingWidget();
    pinfo.settingwidget = swidget;

    setInformation(pinfo);
}

BufferedCopy::~BufferedCopy()
{
    delete swidget;
}

int BufferedCopy::functionMain(int argc, QStringList *args)
{
    if(argc < 2){
        functionMessage(tr("The argument is missing."), MessageType::Error);
        return -1;
    }

    QString source = args->at(0);
    QString dest = args->at(1);
    QString filename = "";
    long cachesize = 524288;
    bool verify = false;

    //get argument data
    QString argcheck;
    for(int i = 2; i < argc; i++){
        argcheck = static_cast<QString>(args->at(i)).toLower();

        if(argcheck == "-rename" && (i + 1) < argc){
            filename = args->at(i + 1);
            i++;
        }

        if(argcheck == "-cs" && (i + 1) < argc){
            bool ok = false;
            int chsize = static_cast<QString>(args->at(i + 1)).toInt(&ok);
            if(ok) cachesize = chsize;
            i++;
        }

        if(argcheck == "-v") verify = true;
    }

    //output filename change
    QFileInfo stmp(source);
    if(filename == "" && stmp.exists()){
        dest = QFileInfo(dest).dir().path() \
                + stmp.fileName();
    }else if(stmp.exists()){
        dest = QFileInfo(dest).dir().path() \
                + filename;
    }else{
        functionMessage(tr("Source file does not exist."), MessageType::Error);
        return -1;
    }

    //execute copyprocess
    if(!copyData(source, dest, cachesize)){
        // copy failed
        return -1;
    }

    //in the case of the "verify" option is not enabled.
    if(!verify){
        functionMessage(tr("File writing completed!"), MessageType::Error);
        return 0;
    }

    if(!checkHash(source,dest)) return -1;

    return 0;
}

bool BufferedCopy::copyData(QString source, QString dest, long cachesize)
{
    //create destination file
    QFile ofile(dest);
    if (!ofile.open(QIODevice::WriteOnly)){
        functionMessage(tr("Destination could not open."), MessageType::Error);
        return false;
    }

    //read source data
    QFile sfile(source);
    if (!sfile.open(QIODevice::ReadOnly)){
        functionMessage(tr("Source file could not open."), MessageType::Error);
        ofile.close();
        return false;
    }

    //copy data and show progress
    QByteArray buffer;

    qint64 filesize = sfile.size();
    qint64 current = 0;

    QString *prevprogress = new QString();
    QString currentprogress = "";

    while(!sfile.atEnd()){
        //set buffer size
        buffer = sfile.read(cachesize);
        ofile.write(buffer);

        current += buffer.size();

        currentprogress = QString::number(static_cast<double>(current)/filesize * 100, 'f', 2);

        if(currentprogress != prevprogress) functionMessage(tr("Progress %1 %").arg(currentprogress), MessageType::Update);

        *prevprogress = currentprogress;
    }
    sfile.close();
    ofile.close();

    delete prevprogress;

    return true;
}

bool BufferedCopy::checkHash(QString source, QString dest)
{
    QByteArray shash = sha1Hash(source).toHex();
    QByteArray dhash = sha1Hash(dest).toHex();

    QString hashresult = tr("fail");
    bool hashres = (shash == dhash)? true : false;
    if(hashres) hashresult = tr("success");

    functionMessage(tr("[%1] Source      : ").arg(this->getInformation()->name) + QString(shash) + tr("\n") + \
                    tr("[%1] Destination : ").arg(this->getInformation()->name) + QString(dhash) + tr("\n") + \
                    tr("[%1] Hash check  : %2").arg(this->getInformation()->name).arg(hashresult) \
                    , MessageType::Update);

    return hashres;
}

QByteArray BufferedCopy::sha1Hash(QString file)
{
    QFile sf(file);
    if (sf.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (hash.addData(&sf)) {
            return hash.result();
        }
    }

    return QByteArray();
}
