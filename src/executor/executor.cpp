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

#include "executor.h"

Executor::Executor(QObject *parent)
    : QObject(parent)
{
    pbuilder = new ProcessXmlBuilder();
    localHash = new QHash<QString, QString>();
    userexeclist.clear();

    work = new WorkingParam();
    test = new TestParam();
    setting = new SettingParam();
}

Executor::~Executor()
{
    delete pbuilder;
    delete localHash;
    delete work;
    delete test;
    delete setting;
//    delete execlist;
}

int Executor::getStartnum() const
{
    return test->startnum;
}

void Executor::setStartnum(int start)
{
    test->startnum = start;
}

int Executor::getEndnum() const
{
    return test->endnum;
}

void Executor::setEndnum(int end)
{
    if(end < pbuilder->count())
        test->endnum = end;
}

void Executor::processWrite(QString code)
{
    work->process->write(code.toLocal8Bit());
}

void Executor::processKill()
{
    work->process->kill();
    emit processMessage(QObject::tr("Process killed."), INPUT);
}

void Executor::setMutex(QMutex *mutex)
{
    //TODO: no imprementation to run function
    sMutex = mutex;
}

bool Executor::getPaused() const
{
    return work->paused;
}

void Executor::setPaused(bool pause)
{
    work->paused = pause;
}

QString Executor::macroConvert(QString part)
{
    QString result;

    //1.global
    result = replaceGlobalMacro(part);

    //2.local
    result = replaceLocalMacro(result);

    //3.file
    return replaceInputMacro(result);
}

void Executor::setExecList(QList<int> data)
{
    if(data.count() == 0){
        userexeclist.clear();
    }else{
        userexeclist = data;
    }
}

bool Executor::getWorking() const
{
    return work->working;
}

void Executor::setProcessFile(QString filepath)
{
    profileloaded = true;
    setting->initFilename = filepath;
}

//void Executor::addInputFiles(QStringList list, int inputmax)
//{
//    if(inputmax == -1) inputmax = INT_MAX;
//    int i = fileHash.count() / 5;
//    QString pre = "$input_";
//    foreach(QString str , list){
//        QFileInfo info(str);
//        if(info.exists()){
//            fileHash[pre + QString::number(i) + "_full$"] = info.canonicalFilePath();
//            fileHash[pre + QString::number(i) + "_dir$"] = info.canonicalPath();
//            fileHash[pre + QString::number(i) + "_name$"] = info.fileName();
//            fileHash[pre + QString::number(i) + "_basename$"] = info.baseName();
//            fileHash[pre + QString::number(i) + "_extension$"] = info.completeSuffix();
//            if(i++ == inputmax) break;
//        }
//    }
//}

void Executor::addInputFiles(QStringList list, int inputmax)
{
    if(inputmax == -1) inputmax = INT_MAX;
    fileList = list;
}

void Executor::setGlobalList()
{
    if(!globalHash.empty()) globalHash.clear();
    StringXmlBuilder *builder = new StringXmlBuilder();
    QList<QStringList> list;

    int counter = builder->count();
    for(int i = 0; i < counter; i++){
        builder->readItem(i, &list);
        globalHash[list.at(0).at(1)] = list.at(1).at(1);
        list.clear();
    }

    delete builder;
}

void Executor::setLocalList()
{
    if(!profileloaded) return;
    if(!localHash->empty()) localHash->clear();

#ifdef QT_DEBUG
    qDebug() << "[Executor::setLocalList]";
#endif

    QList<QStringList> list;

    if(pbuilder->count() > 1 && pbuilder->readItem(LOCALVARINDEX, &list)){
        int localc = xgen.fetch(pxc.TAG_L_VARIANTCOUNT_INT, &list).toInt();

        for(int i = 0; i < localc; i++){
            localHash->insert(list.at(2 + i).at(1), list.at(2 + i).at(3));
        }
    }
}

bool Executor::runProcess()
{
    //local
    int loopcount;
    bool fileinput = false;

    //file input existing check
    if(setting->initFilename == ""){
        emit processCheckError(tr("## [Fatal] The selected profile cannot be loaded."));
        emit processStopped();
        return false;

    }

    //load init xmlfile
    pbuilder->setLoadPath(setting->initFilename);

    //Execution range for ProfileEditor
    execlist = new QList<int>();
    if(userexeclist.count() > 0) execlist->append(userexeclist);
    checkExecList(execlist);
    emit processStateCount(0, execlist->count());

    //Read profile information (Info data must index 0)
    setProcessSettings(&fileinput, &loopcount);

    //Load variables
    setGlobalList();
    setLocalList();

    //Preparation for execution of executable file
    work->process = new QProcess();
    work->process->setProcessChannelMode(QProcess::MergedChannels);

    //Preparing to communicate with the console
    connect(work->process, &QIODevice::readyRead, this, &Executor::loadNormalStandardOutput);

#ifdef QT_DEBUG
    qDebug() << "[Executor::runProcess] execlist count :" << execlist->count();
#endif

    //Execution flags
    work->working = true;

    emit processStarted(setting->launched);

    //Execute main process
    while(loopcount > 0 || loopcount < 0){
        Execute();

        //Reduce the number of fileList and check lest file count
        if(fileinput && fileList.count() > 0){
            for(int i = 0; i < setting->argumentscount; i++) fileList.removeFirst();
            loopcount = (fileList.count() == 0)? 0 : -1;
        }

        if(loopcount > 0) loopcount--;
    }

    //work->working data is reseted in resetdata();
    if(work->working) emit processEnded(setting->launched);

    //Disconnecting communication, erasing generated files, initializing
    resetdata();

    disconnect(work->process, &QIODevice::readyRead, this, &Executor::loadNormalStandardOutput);

    delete work->process;
    delete execlist;

    return true;
}

bool Executor::Execute()
{
    QList<QStringList> *list = new QList<QStringList>();
    bool checker = true;
    int execlistcounter = execlist->count();

    for(int i = 0; i < execlistcounter; i++){

        // if this check results true, work->working.
        if(processStopHandleChecker()){
           emit processStateUpdate(i);
        }else{
            break;
        }

        //read each list
        if(pbuilder->readItem(execlist->at(i), list)){

            FunctionType ft;
            switch (ft.getType(list->at(0).at(1))) {
            case ft.TYPE::INFORMATION: checker = loadInfo(list);    break;
            case ft.TYPE::EXECUTE:     checker = loadNormal(list);  break;
            case ft.TYPE::FILESEARCH:  checker = loadSearch(list);  break;
            case ft.TYPE::PLUGIN:      checker = loadPlugins(list); break;
            case ft.TYPE::PROFILELOAD: checker = loadProject(list); break;
            case ft.TYPE::ALLINCLUDE:  checker = loadTemp(list);    break;
            case ft.TYPE::LOCAL:                                    break;
            case ft.TYPE::INVALID:                                  break;
            }

            list->clear();

            if(!checker) emit processErrorOccured(i);
        }

        // if this check results true, work->working.
        if(!processStopHandleChecker()) break;

    }
    //send max counter of all processcount
    emit processStateUpdate(execlistcounter);

    delete list;
    return checker;
}

void Executor::stopProcess()
{
    work->working = false;
}

void Executor::loadNormalStandardOutput()
{
    //TODO: it needs to detect various application's encode
    QByteArray read = work->process->readAll();
    QString encode = QTextCodec::codecForLocale()->toUnicode(read);
//    QString encode = QTextCodec::codecForUtfText(read, QTextCodec::codecForLocale())->toUnicode(read);
//    QString encode = QTextCodec::codecForUtfText(read)->toUnicode(read);
    emit processMessage(encode, NORMAL);
}

/**
 * @fn Executor::loadInfo
 * @brief it shows the autobatchrunner project description text on the console.
 * @param list processxmlbuilder information list.
 * @return true or false (it no means.)
 */
bool Executor::loadInfo(QList<QStringList> *list)
{
    QString curdata;
    QString result = "";

    // line 1
    result.append(tr("``` \n"));

    // line 2
    curdata = xgen.fetch(pxc.TAG_I_NAME, list);
    curdata = (curdata == "")? "(no name)" : curdata;
    result.append(tr("Project : ")).append(curdata).append(tr(" \n"));

    // line 3
    curdata = xgen.fetch(pxc.TAG_I_VERSION, list);
    curdata = (curdata == "")? "(test)" : curdata;
    result.append(tr("Version : ")).append(curdata).append(tr(" \n"));

    // line 4
    curdata = xgen.fetch(pxc.TAG_I_AUTHOR, list);
    curdata = (curdata == "")? "(anonymous)" : curdata;
    result.append(tr("Created : ")).append(curdata).append(tr(" \n"));

    //line 5
    result.append(tr("``` \n"));

    emit processMessage(result, INFO);

    return true;
}

bool Executor::loadNormal(QList<QStringList> *list)
{
    FunctionType ft;
    //scheduler only or not
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_TYPE, ft.getString(ft.TYPE::EXECUTE), pxc.ATTR_ONLY_SCHEDULER_BOOL, list))
            && setting->launched == DEFAULT) return true;

    int cmdc = static_cast<QString>(xgen.fetch(pxc.TAG_E_COMMANDCOUNT_INT, list)).toInt();

    if(cmdc == 0){
        emit processMessage(tr("## [Execution] No execution command, it skips."), ERROR);
        return true;
    }

    //set args
    QString app;
    QStringList arguments;
    int cmdfirst = xgen.fetchCmdFirstPos(pxc.TAG_E_CMD_HA1, list);

    app = macroConvert(list->at(cmdfirst).at(1));

    for(int i = 1; i < cmdc; i++){
        arguments.append(macroConvert(list->at(cmdfirst + i).at(1)));
    }

    //show cmd lists
    QString show;
    QFileInfo apps(app);
    if(apps.isFile()){
        show.append(tr("## [") + apps.fileName() + tr("] \n"));
    }else{
        show.append(tr("## [") + app + tr("] \n"));
    }

    for(QString var : arguments) {
        show.append(" ");
        show.append(var);
    }
    show.append("\n\n").append("```");

    emit processMessage(show, NORMAL);

    //start commands
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_E_DETACH_BOOL, list))){
        work->process->startDetached(app, arguments);
    }else{
        work->process->start(app, arguments);
    }

    //wait commands
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_E_TIMEOUT_BOOL_HA1, list))){
        work->process->waitForFinished( \
                    static_cast<QString>(xgen.fetch(pxc.TAG_E_TIMEOUT_BOOL_HA1,pxc.ATTR_TIMEOUT_INT, list)).toInt());
    }else{
        work->process->waitForFinished(-1);
    }

    show.clear();
    show.append("```").append("\n");
    emit processMessage(show, NORMAL);

    return true;
}

bool Executor::loadSearch(QList<QStringList> *list)
{
    FunctionType ft;
    //scheduler only or not
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_TYPE, ft.getString(ft.TYPE::FILESEARCH), pxc.ATTR_ONLY_SCHEDULER_BOOL, list))
            && setting->launched == DEFAULT) return true;

    FileSearchLoader *loader = new FileSearchLoader();
    QStringList result = loader->searchFromXml( \
                static_cast<QString>(xgen.fetch(pxc.TAG_FS_NAME_HA1,pxc.ATTR_COMMAND_ID_INT, list)).toInt());

    emit processMessage(tr("## [FileSearch : %1] \n").arg(xgen.fetch(pxc.TAG_FS_NAME_HA1, list)) + \
                        QString::number(result.count()) + tr(" files found.\n"), SEARCH);

    //TODO : separation data detection ?
    //combine search result
    QString combineresult;
    QString sepdata = xgen.fetch(pxc.TAG_FS_SEPARATOR, list);

    //convert sepdata
    sepdata = sepdata.contains("\\r\\n") ? sepdata.replace("\\r\\n","\r\n") : sepdata;
    sepdata = sepdata.contains("\\n")    ? sepdata.replace("\\n","\n")      : sepdata;
    sepdata = sepdata.contains("\\r")    ? sepdata.replace("\\r","\r")      : sepdata;
    sepdata = sepdata.contains("\\t")    ? sepdata.replace("\\t","\t")      : sepdata;

    int cre = result.count();
    for(int i = 0; i < cre; i++){
        emit processMessage(tr("%1").arg(result.at(i)), SEARCH);

        combineresult.append(result.at(i));
        if(i < (cre-1)){
            combineresult += sepdata;
        }
    }

    int radiodata = static_cast<QString>( \
                xgen.fetch(pxc.TAG_FS_FILEPATH_HA1, pxc.ATTR_FS_OUTPUTOPTION_INT, list)).toInt();

    //save to variant
    if(radiodata == 0){
        QString selectvar = xgen.fetch(pxc.TAG_FS_VARIANT, list);

        if(selectvar != ""){
            //set data to variant
            overwriteLocalMacro(selectvar, combineresult);
        }else{
            emit processMessage(tr("## [FileSearch] The selected variant cannot be found."), ERROR);
        }

    }else{
        //TODO: create data to File
        QString outputfile = xgen.fetch(pxc.TAG_FS_FILEPATH_HA1, list);

        if(outputfile != ""){
            // 0: overwrite, 1: append
            int writetype = xgen.fetch(pxc.TAG_FS_WRITEOPTION_INT, list).toInt();
            bool opened = false;

            QFile file(outputfile);

            //TODO: add overwrite permission (2017/09/02 updated)
//            if(file.exists() && setting->searchoutputoverwrite){
//                //clear file string
//                file.resize(0);
//            }

            if(file.exists()){
                switch (writetype) {
                case 0:
                    //clear file string
                    file.resize(0);
                    opened = file.open(QIODevice::ReadWrite | QFile::Text);
                    break;
                case 1:
                    opened = file.open(QIODevice::Append | QFile::Text);
                    combineresult.insert(0, sepdata);
                    break;
                default:
                    break;
                }

            }else{
                opened = file.open(QIODevice::ReadWrite | QFile::Text);
            }

            //create new file
            if(opened){
                QTextStream stream(&file);
                stream << combineresult;
                file.close();
            }

        }
    }

    delete loader;
    return true;
}

bool Executor::loadPlugins(QList<QStringList> *list)
{
    //scheduler only or not
    FunctionType ft;
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_TYPE, ft.getString(ft.TYPE::PLUGIN), pxc.ATTR_ONLY_SCHEDULER_BOOL, list))
            && setting->launched == DEFAULT) return true;

    QString file = xgen.fetch(pxc.TAG_P_FILEPATH, list);
    QFileInfo info(file);

    if(!info.exists()) return false;

    //check plugin can use
    QPluginLoader loader(file);

    if(!loader.load()) return false;

    QObject *plugin = loader.instance();
    ext = qobject_cast<ExtraPluginInterface *>(plugin);
    connect(ext, &ExtraPluginInterface::updateMessage, this, &Executor::sendPluginMessage);

    int cmdc = static_cast<QString>(xgen.fetch(pxc.TAG_P_COMMANDCOUNT_INT, list)).toInt();
    int ecmdfirst = xgen.fetchCmdFirstPos(pxc.TAG_P_CMD_HA1, list);

    QStringList tmp;
    for(int i = 0; i < cmdc; i++){
        tmp.append(macroConvert(list->at(ecmdfirst + i).at(1)));
    }

    ext->setValue(&fileList, InputType::File);
    ext->setValue(&globalHash, InputType::Global);
    ext->setValue(localHash, InputType::Local);

    bool result = true;

    const QString plname = ext->getInformation()->name;

    emit processMessage(tr("## [Plugin] %1 started.").arg(plname), PLUGINS);

    if(!ext->functionMain(cmdc, &tmp)){
        QString success = ext->getMessage(MessageType::Success);
        if(success == "") success = tr("successfully completed.");

        emit processMessage(tr("## [Plugin] %1 %2").arg(plname).arg(success), PLUGINS);

    }else{
        QString error = ext->getMessage(MessageType::Error);
        if(error == "") error = tr("failed.");

        emit processMessage(tr("## [Plugin] %1 %2").arg(plname).arg(error), PLUGINS);
        result = false;
    }
    disconnect(ext, &ExtraPluginInterface::updateMessage, this, &Executor::sendPluginMessage);

    loader.unload();

    return result;
}

bool Executor::loadProject(QList<QStringList> *list)
{
    FunctionType ft;
    //scheduler only or not
    if(VariantConverter::stringToBool(xgen.fetch(pxc.TAG_TYPE, ft.getString(ft.TYPE::PROFILELOAD), pxc.ATTR_ONLY_SCHEDULER_BOOL, list))
            && setting->launched == DEFAULT) return true;

    QFileInfo info(xgen.fetch(pxc.TAG_PLOAD_FILEPATH, list));
    if(!info.exists()){
        emit processMessage(tr("## [ProfileLoad] %1 cannot be loaded.")
                            .arg(info.fileName()), ERROR);
        return false;
    }

    //stack currentdata
    builderstack.push(pbuilder);
    localstack.push(localHash);
    execliststack.push(execlist);

    //check nest
    if(builderstack.count() >= setting->othernestmax) neststop = true;

    //alternate data
    pbuilder = new ProcessXmlBuilder();
    pbuilder->setLoadPath(xgen.fetch(pxc.TAG_PLOAD_FILEPATH, list));

    execlist = new QList<int>();
    checkExecList(execlist);

    localHash = new QHash<QString, QString>();
    setLocalList();

    //set counter
    int counter = execlist->count();

    emit processMessage(tr("## [ProfileLoad] %1 (loop %2)")
                        .arg(xgen.fetch(pxc.TAG_PLOAD_FILEPATH, list))
                        .arg(QString::number(builderstack.count()))
                        , OTHER);

    //init data
    QList<QStringList> *ilist = new QList<QStringList>();
    bool checker = true;

    for(int i = 0; i < counter; i++){
        ilist->clear();

        qDebug() << "[Executor::loadOther] otherexeclist count : " << counter;

        //read each list
        if(pbuilder->readItem(execlist->at(i), ilist)){

            //scheduler only or not
            FunctionType ft;
            switch (ft.getType(ilist->at(0).at(1))) {
            case FunctionType::TYPE::INFORMATION: checker = loadInfo(ilist);    break;
            case ft.TYPE::EXECUTE:     checker = loadNormal(ilist);  break;
            case ft.TYPE::FILESEARCH:  checker = loadSearch(ilist);  break;
            case ft.TYPE::PLUGIN:      checker = loadPlugins(ilist); break;
            case ft.TYPE::PROFILELOAD: checker = neststop ? false : loadProject(ilist); break;
            case ft.TYPE::ALLINCLUDE:  checker = loadTemp(ilist);    break;
            case ft.TYPE::LOCAL:                                     break;
            case ft.TYPE::INVALID:                                   break;
            }

            if(!checker){
                // when max nested, all processes are through.
                i = counter;
            }
        }
    }

    //delete data
    delete ilist;
    delete pbuilder;
    delete execlist;
    delete localHash;

    //pop data
    pbuilder = builderstack.pop();
    execlist = execliststack.pop();
    localHash = localstack.pop();

    //check nest
    if(builderstack.count() == 0) neststop = false;

    return true;
}

bool Executor::loadTemp(QList<QStringList> *list)
{
    //TODO: dynamic index update
    TAB selected = static_cast<TAB>(xgen.fetch(pxc.TAG_FUNCTIONSELECT, list).toInt());

    switch (selected) {
    case TAB::EXECUTE:     return loadNormal(list);
    case TAB::FILESEARCH:  return loadSearch(list);
    case TAB::PLUGINS:     return loadPlugins(list);
    case TAB::PROFILELOAD: return ((neststop)? false : loadProject(list));
    case TAB::INVALID:     return false;
    }

    return false;
}

QString Executor::replaceInputMacro(QString original)
{
    QString pre = "$input_";
    QString result = original;

    int maxcount = fileList.count();

    for(int i = 0; i < maxcount; i++){
        QFileInfo info(fileList.at(i));

        result.replace(pre + QString::number(i) + "_full$", info.canonicalFilePath());
        result.replace(pre + QString::number(i) + "_dir$", info.canonicalPath());
        result.replace(pre + QString::number(i) + "_name$", info.fileName());
        result.replace(pre + QString::number(i) + "_basename$", info.baseName());
        result.replace(pre + QString::number(i) + "_extension$", info.completeSuffix());
        if(info.isSymLink())
            result.replace(pre + QString::number(i) + "_symlink$", info.symLinkTarget());
    }

    return result;
}

QString Executor::replaceGlobalMacro(QString original)
{
    return replaceMacro(original, &globalHash);
}

QString Executor::replaceLocalMacro(QString original)
{
    return replaceMacro(original, localHash);
}

QString Executor::replaceMacro(QString original, QHash<QString, QString> *list)
{
    QString result = original;
    QList<QString> il = list->keys();
    int sizer = il.size();
    for(int i = 0; i < sizer; i++){
        result = result.replace(il.at(i), list->value(il.at(i)));
    }

    return result;
}

void Executor::overwriteLocalMacro(QString key, QString value)
{
    if(localHash->contains(key)) localHash->insert(key, value);
}

void Executor::checkExecList(QList<int> *elist)
{
    int excount = elist->count();
    int buildermax = pbuilder->count();

    if(excount == 0){
        for(int i = 0; i < buildermax; i++){
            elist->append(i);
        }
        noexeclist = true;

    }else{
        for(int i = 0; i < excount; i++){
            if(!(elist->at(i) < buildermax)){
                emit processMessage(tr("Index ")
                                    + QString::number(elist->at(i))
                                    + tr(" is not exist. \n"), ERROR);
                elist->removeAt(i);
            }
        }
    }
}

void Executor::setProcessSettings(bool *fileinput, int *loopcount)
{
    QList<QStringList> list;
    if(!pbuilder->readItem(0, &list)) return;

    //File reading setting (manual file input)
    if(VariantConverter::stringToBool( \
                xgen.fetch(pxc.TAG_I_FILEINPUT_BOOL, &list))){
        *fileinput = true;
    }

    //search file input
    if(VariantConverter::stringToBool( \
                xgen.fetch(pxc.TAG_I_FILEINPUT_SEARCH_BOOL, &list))){
        *fileinput = true;
        FileSearchLoader loader;
        fileList.append(loader.searchFromXml( \
                        static_cast<QString>(xgen.fetch(pxc.TAG_I_FILESEARCH_NAME, pxc.ATTR_COMMAND_ID_INT, &list)).toInt()));
        emit processMessage(tr("Input : ") + QString::number(fileList.count()) + tr(" files.\n"), SEARCH);
    }

    //Whether to loop the read file to the end ( 1 : no loop or set max loop count )
    *loopcount = VariantConverter::stringToBool(xgen.fetch(pxc.TAG_I_PROCESS_BOOL_HA1, &list)) ? \
                1 : static_cast<QString>(xgen.fetch(pxc.TAG_I_PROCESS_BOOL_HA1, pxc.ATTR_I_PROCESSMAX_INT, &list)).toInt();

    // Number of files used at one time
    setting->argumentscount = static_cast<QString>( \
                xgen.fetch(pxc.TAG_I_ARG_IN_ONELOOP_INT, &list)).toInt();

    //load other profile nest counter
    setting->othernestmax = static_cast<QString>( \
                xgen.fetch(pxc.TAG_I_RECURSIVE_LOOPMAX_INT, &list)).toInt();


    qDebug() << "[Executor::setProcessSettings] Settings loaded";
}

void Executor::resetdata()
{
#ifdef QT_DEBUG
    qDebug() << "[Executor::resetdata]";
#endif

    //reset flags
    work->working = false;
    if(setting->launched == DEFAULT){
        profileloaded = false;
        pbuilder->setLoadPath("");
    }
    if(noexeclist){
        execlist->clear();
        noexeclist = false;
    }
}

void Executor::sleep(int ms)
{
    QEventLoop loop;
    QTimer::singleShot( ms, &loop, SLOT( quit() ) );
    loop.exec();
}

bool Executor::processStopHandleChecker()
{
    if(work->paused) emit processPaused();

    while (work->paused && work->working){
        //infinity loop wait 50 ms
        sleep(50);
    }

    //check stop check
    if(!work->working){
        emit processStopped();
    }

    return work->working;
}

int Executor::getLaunchedfrom() const
{
    return setting->launched;
}

void Executor::setLaunchedfrom(int only)
{
    setting->launched = only;
}
