#include "executor.h"

#include <QFileInfo>

Executor::Executor(QObject *parent)
    : QObject(parent)
{
    pbuilder = new ProcessXmlBuilder();
    localHash = new QHash<QString, QString>();
//    execlist = new QList<int>();
    userexeclist.clear();

//    connect(this, &Executor::processEnded, this, &Executor::processEndLastStatus);
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

bool Executor::getDetached() const
{
    return setting->detached;
}

void Executor::setDetached(bool detach)
{
    setting->detached = detach;
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
//    emit processMessage(code, INPUT);
}

void Executor::processKill()
{
    work->process->kill();
    emit processMessage(QObject::tr("Process killed."), INPUT);
    //    emit processStopped();
}

void Executor::setMutex(QMutex *mutex)
{
    //TODO: no imprementation to run function
    sMutex = mutex;
}

//int Executor::getForcequittime() const
//{
//    return forcequittime;
//}

//void Executor::setForcequittime(int ms)
//{
//    forcequittime = ms;
//}

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
//    pbuilder->setLoadPath(filepath);
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
    qDebug() << "Executor::setLocalList";
#endif

    int counter = pbuilder->count();
    QList<QStringList> list;

    for(int i = 0; i < counter; i++){
        if(pbuilder->readItem(i, &list) && getReadType(list.at(0).at(1)) == 6){
            int localc = ((QString)list.at(1).at(1)).toInt();

            for(int i = 0; i < localc; i++){
                localHash->insert(list.at(2 + i).at(0), list.at(2 + i).at(1));
            }
            break;
        }
    }
}

//bool Executor::runProcess()
//{
//    emit processStarted(MAINPROCESS);

//    //check state
//    if(!profileloaded){
//        emit processCheckError(tr("xmlfile not loaded!"));
//        emit processStopped();
//        return false;
//    }

//    //check counter
//    //TODO:
////    if(test->endnum < 0 || test->endnum > pbuilder->count()) test->endnum = pbuilder->count();
//    //emit maximum count number
//    execlist = new QList<int>();
//    if(userexeclist.count() > 0) execlist->append(userexeclist);
//    checkExecList(execlist);

////    emit processStateCount(test->startnum, test->endnum);
//    emit processStateCount(0, execlist->count());

//    //init data
//    QList<QStringList> *list = new QList<QStringList>();
//    bool checker = true;

//    //init process
//    work->process = new QProcess();
//    work->process->setProcessChannelMode(QProcess::MergedChannels);
//    connect(work->process, SIGNAL(readyRead()), this, SLOT(loadNormalStandardOutput()));

//    //set work->working
//    work->working = true;


//    int execlistcounter = execlist->count();
//#ifdef QT_DEBUG
//    qDebug() << "Executor:: execlist count" << execlistcounter;
//#endif

//    for(int i = 0; i < execlistcounter; i++){

//        // if this check results true, work->working.
//        if(processStopHandleChecker()){
//           emit processStateUpdate(i);
//        }else{
//            break;
//        }

//        //read each list
//        if(pbuilder->readItem(execlist->at(i), list)){

//            //scheduler only or not
//            if(VariantConverter::stringToBool(list->at(0).at(3)) && setting->launched == DEFAULT) break;

//            switch (getReadType(list->at(0).at(1))) {
//            case INFO:
//                checker = loadInfo(list, 1);
//                break;

//            case NORMAL:
//                checker = loadNormal(list, 0);
//                break;

//            case SEARCH:
//                checker = loadSearch(list, 0);
//                break;

//            case SCRIPT:
//                checker = loadScript(list, 0);
//                break;

//            case OTHER:
//                checker = loadOther(list, 0);
//                break;

//            case TEMP:
//                checker = loadTemp(list);
//                break;

//            case LOCAL:
//            default:
//                break;
//            }

//            list->clear();

//            if(!checker){
//                emit processErrorOccured(i);
//            }
//        }

//        // if this check results true, work->working.
//        if(!processStopHandleChecker()) break;

//    }
//    //send max counter of all processcount
//    emit processStateUpdate(execlistcounter);

//    //work->working data is reseted in resetdata();
//    if(work->working) emit processEnded(MAINPROCESS);

//    //load file etc force reset.
//    resetdata();

//    //reset process
//    disconnect(work->process, SIGNAL(readyRead()), this, SLOT(loadNormalStandardOutput()));
//    delete work->process;
//    delete execlist;
//    delete list;

//    return true;
//}

bool Executor::runProcess()
{
    //local
    int loopcount;
    bool fileinput = false;

    //file input existing check
    if(setting->initFilename == ""){
        emit processCheckError(tr("xmlfile not loaded!"));
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

    //Preparation for execution of executable file
    work->process = new QProcess();
    work->process->setProcessChannelMode(QProcess::MergedChannels);

    //Preparing to communicate with the console
    connect(work->process, &QIODevice::readyRead, this, &Executor::loadNormalStandardOutput);

    #ifdef QT_DEBUG
        qDebug() << "Executor:: execlist count" << execlist->count();
    #endif

    //Read profile information (Info data must index 0)
    setProcessSettings(&fileinput, &loopcount);

    //Load variables
    setGlobalList();
    setLocalList();

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

            switch (getReadType(list->at(0).at(1))) {
            case INFO:
                checker = loadInfo(list, 1);
                break;

            case NORMAL:
                checker = loadNormal(list, 0);
                break;

            case SEARCH:
                checker = loadSearch(list, 0);
                break;

            case SCRIPT:
                checker = loadPlugins(list, 0);
                break;

            case OTHER:
                checker = loadOther(list, 0);
                break;

            case TEMP:
                checker = loadTemp(list);
                break;

            case LOCAL:
            default:
                break;
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

//void Executor::pauseProcess()
//{
//    paused = true;
//}

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

//void Executor::processEndLastStatus(int maxcount)
//{
//#ifdef QT_DEBUG
//    qDebug() << "Executor:: processEndLastStatus()" << maxcount;
//#endif
//    emit processStateUpdate(maxcount);
//}

//Think kinds of text
bool Executor::loadInfo(QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QString result = "";
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;

    // line 1
    result.append(tr("Project : "));
    result.append(curdata);
    result.append(" : ver.");

    curdata = list->at(firstpos + 1).at(1);
    curdata = (curdata == "")? "test" : curdata;
    result.append(curdata);
//    result.append(" - last update ");
//    result.append(list->at(firstpos + 4).at(1));
    result.append(tr("\r\n"));

    // line 2
    curdata = list->at(firstpos + 2).at(1);
    curdata = (curdata == "")? "Anonymous" : curdata;

    result.append(tr("This process is created by "));
    result.append(curdata);
    result.append(tr(".\r\n"));

    //line 3
    result.append(tr("--------------------------------------------------"));
    result.append(tr("\r\n"));

    emit processMessage(result, INFO);

    //load other profile nest counter
//    setting->othernestmax = ((QString)list->at(firstpos + 9).at(1)).toInt();

    return true;
}

bool Executor::loadNormal(QList<QStringList> *list, int firstpos)
{
    //scheduler only or not
    if(VariantConverter::stringToBool(list->at(firstpos).at(3))
            && setting->launched == DEFAULT) return true;

    int cmdc = ((QString)list->at(firstpos + 2).at(1)).toInt();

    if(cmdc == 0){
        emit processMessage(tr("No executable file. Skip."), ERROR);
        return true;
    }

    //set args
    QString app;
    QStringList arguments;

    app = macroConvert(list->at(firstpos + 3).at(1));

    for(int i = 1; i < cmdc; i++){
        arguments.append(macroConvert(list->at(firstpos + 3 + i).at(1)));
    }

    //show cmd lists
    QString show;
    QFileInfo apps(app);
    if(apps.isFile()){
        show.append(apps.fileName());
    }else{
        show.append(app);
    }

    foreach (QString var, arguments) {
        show.append(" ");
        show.append(var);
    }
    show.append("\r\n");

    emit processMessage(show, NORMAL);

    //start commands
    if(setting->detached){
        work->process->startDetached(app, arguments);
    }else{
        work->process->start(app, arguments);
    }

    //wait commands
    if(VariantConverter::stringToBool(list->at(firstpos + 1).at(1))){
        work->process->waitForFinished(((QString)list->at(firstpos + 1).at(3)).toInt());
    }else{
        work->process->waitForFinished(-1);
    }

    return true;
}

bool Executor::loadSearch(QList<QStringList> *list, int firstpos)
{
    //scheduler only or not
    if(VariantConverter::stringToBool(list->at(firstpos).at(3))
            && setting->launched == DEFAULT) return true;

    FileSearchLoader *loader = new FileSearchLoader();
    QStringList result = loader->searchFromXml(((QString)list->at(firstpos + 1).at(3)).toInt());

    emit processMessage(tr("Search : ") + QString::number(result.count()) + tr(" files found.\r\n"), SEARCH);

    //TODO : separation data detection ?
    //combine search result
    QString combineresult;
    QString sepdata = list->at(firstpos + 2).at(1);
    int cre = result.count();
    for(int i = 0; i < cre; i++){
        emit processMessage(tr("%1").arg(result.at(i)), SEARCH);

        combineresult.append(result.at(i));
        if(i < (cre-1)){
            if(sepdata.contains("\\r\\n")){
                combineresult += "\r\n";

            }else if(sepdata.contains("\\n")){
                combineresult += '\n';

            }else if(sepdata.contains("\\r")){
                combineresult += '\r';

            }else if(sepdata.contains("\\t")){
                combineresult += '\t';

            }else{
                combineresult += sepdata;

            }
        }
    }

    int radiodata = ((QString)list->at(firstpos + 4).at(3)).toInt();

    //save to variant
    if(radiodata == 0){
        QString selectvar = list->at(firstpos + 3).at(1);

        if(selectvar != ""){
            //set data to variant
            overwriteLocalMacro(selectvar, combineresult);
        }else{
            emit processMessage(tr("Search : No variant is defined."), ERROR);
        }

    }else{
        //TODO: create data to File
        QString outputfile = list->at(firstpos + 4).at(1);

        if(outputfile != ""){
            QFile file(outputfile);

            //TODO: add overwrite permission (2017/09/02 updated)
            if(file.exists() && setting->searchoutputoverwrite){
                //clear file string
                file.resize(0);
            }

            //create new file
            if(file.open(QIODevice::ReadWrite | QFile::Text)){
                QTextStream stream(&file);
                stream << combineresult;
                file.close();
            }
        }
    }

    delete loader;
    return true;
}

bool Executor::loadPlugins(QList<QStringList> *list, int firstpos)
{
    //scheduler only or not
    if(VariantConverter::stringToBool(list->at(firstpos).at(3))
            && setting->launched == DEFAULT) return true;

    QString file = list->at(firstpos + 2).at(1);
    QFileInfo info(file);
    bool result = true;

    if(info.exists()){
        //check plugin can use
        QPluginLoader loader(file);

        if(loader.load()){
            QObject *plugin = loader.instance();
            ext = qobject_cast<ExtraPluginInterface *>(plugin);

//            connect(plugin, SIGNAL(sendMessage()), this, &Executor::extrafuncInternalMessage);

            int cmdc = ((QString)list->at(firstpos + 3).at(1)).toInt();

            QStringList tmp;
            for(int i = 0; i < cmdc; i++){
                tmp.append(macroConvert(list->at(firstpos + 4 + i).at(1)));
            }

            ext->setInputFileData(fileList);
            ext->setGlobalValue(globalHash);
            ext->setLocalValue(localHash);

//            plugin->disconnect(ext, &ExtraPluginInterface::sendMessage
//                       , this, &Executor::extrafuncInternalMessage);

            if(!ext->functionMain(cmdc, &tmp)){
                emit processMessage(plugin->metaObject()->className() + tr(" : function sucessful.\r\n"), SCRIPT);

            }else{
                emit processMessage(tr("Plugin function failed.\r\n"), SCRIPT);
                result = false;
            }
            loader.unload();

        }else{
            result = false;
        }

    }else{
        result = false;

    }
    return result;
}

bool Executor::loadOther(QList<QStringList> *list, int firstpos)
{
//    emit processStarted(OTHERPROCESS);
    //scheduler only or not
    if(VariantConverter::stringToBool(list->at(firstpos).at(3))
            && setting->launched == DEFAULT) return true;

    QFileInfo info(list->at(firstpos + 2).at(1));
    if(!info.exists()){
        emit processMessage(tr("Other Process : %1 is not existed.")
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
    pbuilder->setLoadPath(list->at(firstpos + 2).at(1));

    execlist = new QList<int>();
    checkExecList(execlist);

    localHash = new QHash<QString, QString>();
    setLocalList();

    //set counter
    int counter = execlist->count();

    emit processMessage(tr("Other Process : %1 (loop %2)")
                        .arg(list->at(firstpos + 1).at(1))
                        .arg(QString::number(builderstack.count()))
                        , OTHER);

    //init data
    QList<QStringList> *ilist = new QList<QStringList>();
    bool checker = true;

    for(int i = 0; i < counter; i++){
        ilist->clear();

        qDebug() << "HINT :: Executor:: otherexeclist count" << counter;

        //read each list
        if(pbuilder->readItem(execlist->at(i), ilist)){

            //scheduler only or not
//            if(VariantConverter::stringToBool(ilist->at(0).at(3)) && setting->launched == DEFAULT) break;

            switch (getReadType(ilist->at(0).at(1))) {
            case INFO:
                checker = loadInfo(ilist, 1);
                break;

            case NORMAL:
                checker = loadNormal(ilist, 0);
                break;

            case SEARCH:
                checker = loadSearch(ilist, 0);
                break;

            case SCRIPT:
                checker = loadPlugins(ilist, 0);
                break;

            case OTHER:
                checker = neststop ? false : loadOther(ilist, 0);
                break;

            case TEMP:
                checker = loadTemp(ilist);
                break;

            case LOCAL:
            default:
                break;
            }

            if(!checker){
//                emit processErrorOccured(i);
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

//    emit processEnded(OTHERPROCESS);

    return true;
}

bool Executor::loadTemp(QList<QStringList> *list)
{
    //TODO: dynamic index update
    int selected = ((QString)list->at(1).at(1)).toInt();

    QHash<int, int> hlist;
    xgen.getListStructure(list, &hlist);

    switch (selected) {
    case ProcessXmlListGenerator::NORMAL/*normal*/: return loadNormal(list, hlist.value(ProcessXmlListGenerator::NORMAL));

    case ProcessXmlListGenerator::SEARCH/*search*/: return loadSearch(list, hlist.value(ProcessXmlListGenerator::SEARCH));

    case ProcessXmlListGenerator::EXTRAFUNC/*script*/: return loadPlugins(list, hlist.value(ProcessXmlListGenerator::EXTRAFUNC));

    case ProcessXmlListGenerator::OTHER/*other*/:
        if(neststop){
            return false;
        }else{
            return loadOther(list, hlist.value(ProcessXmlListGenerator::OTHER));
        }
    default:          return true;
    }
}

// original is part of 1 commands in one process
//QString Executor::replaceInputMacro(QString original)
//{
//    return replaceMacro(original, &fileHash);
//}

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
//        if(original.contains(il.at(i))){
            result = result.replace(il.at(i), list->value(il.at(i)));
//        }
    }

//    QHash<QString, QString>::iterator i = list->find(original);
//    while (i != list->end()) {
//        if(original.contains(i.key())){
//            result = result.replace(original, i.value());
//        }
//        ++i;
//    }

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
                                    + tr(" is not exist. \r\n"), ERROR);
                elist->removeAt(i);
            }
        }
    }
}

void Executor::setProcessSettings(bool *fileinput, int *loopcount)
{
    QList<QStringList> list;
    if(pbuilder->readItem(0, &list)){
        //File reading setting (manual file input)
        if(VariantConverter::stringToBool(list.at(5).at(1))){
            *fileinput = true;
        }

        //search file input
        if(VariantConverter::stringToBool(list.at(6).at(1))){
            *fileinput = true;
            FileSearchLoader loader;
            fileList.append(loader.searchFromXml(((QString)list.at(7).at(3)).toInt()));
            emit processMessage(tr("Input : ") + QString::number(fileList.count()) + tr(" files.\r\n"), SEARCH);
        }

        //Whether to loop the read file to the end ( 1 : no loop or set max loop count )
        *loopcount = VariantConverter::stringToBool(list.at(8).at(1)) ? 1 : ((QString)list.at(8).at(3)).toInt();

        // Number of files used at one time
        setting->argumentscount = ((QString)list.at(9).at(1)).toInt();

        //load other profile nest counter
        setting->othernestmax = ((QString)list.at(10).at(1)).toInt();

    }
}

int Executor::getReadType(QString type)
{
    if(type == TYPE_INFO) return INFO;
    if(type == TYPE_EXEC) return NORMAL;
    if(type == TYPE_SEARCH) return SEARCH;
    if(type == TYPE_SCRIPT) return SCRIPT;
    if(type == TYPE_ANOTHER) return OTHER;
    if(type == TYPE_ALLINCLUDE) return TEMP;
    if(type == TYPE_LOCAL) return LOCAL;
    return -1;
}

void Executor::resetdata()
{
#ifdef QT_DEBUG
    qDebug() << "Executor::resetdata";
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

//int Executor::getOthernestmax() const
//{
//    return setting->othernestmax;
//}

//void Executor::setOthernestmax(int nest)
//{
//    setting->othernestmax = nest;
//}

bool Executor::getSearchfileoverwrite() const
{
    return setting->searchoutputoverwrite;
}

void Executor::setSearchfileoverwrite(bool overwrite)
{
    setting->searchoutputoverwrite = overwrite;
}

int Executor::getLaunchedfrom() const
{
    return setting->launched;
}

void Executor::setLaunchedfrom(int only)
{
    setting->launched = only;
}
