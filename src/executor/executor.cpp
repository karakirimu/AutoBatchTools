#include "executor.h"

#include <QFileInfo>

Executor::Executor(QObject *parent)
    : QObject(parent)
{
    pbuilder = new ProcessXmlBuilder();
    localHash = new QHash<QString, QString>();

    connect(this, &Executor::processEnded, this, &Executor::processEndLastStatus);
}

Executor::~Executor()
{
    delete pbuilder;
    delete localHash;
}

bool Executor::getDetached() const
{
    return detached;
}

void Executor::setDetached(bool detach)
{
    detached = detach;
}

int Executor::getStartnum() const
{
    return startnum;
}

void Executor::setStartnum(int start)
{
    startnum = start;
}

int Executor::getEndnum() const
{
    return endnum;
}

void Executor::setEndnum(int end)
{
    if(end < pbuilder->count())
        endnum = end;
}

int Executor::getForcequittime() const
{
    return forcequittime;
}

void Executor::setForcequittime(int ms)
{
    forcequittime = ms;
}

bool Executor::getPaused() const
{
    return paused;
}

void Executor::setPaused(bool pause)
{
    paused = pause;
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
    execlist = data;
}

bool Executor::getWorking() const
{
    return working;
}

void Executor::setProcessFile(QString filepath)
{
    pbuilder->setLoadPath(filepath);
    processfileloaded = true;
}

void Executor::addInputFiles(QStringList list, int inputmax)
{
    int i = fileHash.count() / 5;
    foreach(QString str , list){
        QFileInfo info(str);
        if(info.exists()){
            fileHash["$input_" + QString::number(i) + "_full$"] = info.canonicalFilePath();
            fileHash["$input_" + QString::number(i) + "_dir$"] = info.canonicalPath();
            fileHash["$input_" + QString::number(i) + "_name$"] = info.fileName();
            fileHash["$input_" + QString::number(i) + "_basename$"] = info.baseName();
            fileHash["$input" + QString::number(i) + "_extension$"] = info.completeSuffix();
            if(i++ == inputmax) break;
        }
    }
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
    if(!processfileloaded) return;
    if(!localHash->empty()) localHash->clear();

    int counter = pbuilder->count();
    QList<QStringList> list;

    for(int i = 0; i < counter; i++){
        if(pbuilder->readItem(i, &list) && getReadType(list.at(0).at(1)) == 6){
            int localc = VariantConverter::stringToInt(list.at(1).at(1));

            for(int i = 0; i < localc; i++){
                localHash->insert(list.at(2 + i).at(0), list.at(2 + i).at(1));
            }
            break;
        }
    }
}

bool Executor::runProcess()
{
    emit processStarted(MAINPROCESS);

    //check state
    if(!processfileloaded){
        emit processCheckError(tr("xmlfile not loaded!"));
        emit processStopped();
        return false;
    }

    //check counter
    //TODO:
//    if(endnum < 0 || endnum > pbuilder->count()) endnum = pbuilder->count();
    //emit maximum count number
    checkExecList();

//    emit processStateCount(startnum, endnum);
    emit processStateCount(0, execlist.count());

    //init data
    QList<QStringList> *list = new QList<QStringList>();
    bool checker = true;

    //init process
    process = new QProcess();
    connect(process, SIGNAL(readyRead()), this, SLOT(loadNormalStandardOutput()));

    //set working
    working = true;


//    for(int i = startnum; i < endnum; i++){
    foreach(int i, execlist){

        if(paused) emit processPaused();

        while (paused && working){
            //infinity loop wait 50 ms
            sleep(50);
        }

        //check stop check
        if(!working){
            emit processStopped();
            break;
        }

        emit processStateUpdate(i);


        //read each list
        if(pbuilder->readItem(i, list)){

            //scheduler only or not
            if(VariantConverter::stringToBool(list->at(0).at(3)) && launchedfrom == DEFAULT) break;

            switch (getReadType(list->at(0).at(1))) {
            case INFO:
                checker = loadInfo(list, 1);
                break;

            case NORMAL:
                checker = loadNormal(list, 1);
                break;

            case SEARCH:
                checker = loadSearch(list, 1);
                break;

            case SCRIPT:
                checker = loadScript(list, 1);
                break;

            case OTHER:
                checker = loadOther(list, 1);
                break;

            case TEMP:
                checker = loadTemp(list);
                break;

            case LOCAL:
            default:
                break;
            }

            list->clear();

            if(!checker){
                emit processErrorOccured(i);
            }
        }
    }

    //reset process
    disconnect(process, SIGNAL(readyRead()), this, SLOT(loadNormalStandardOutput()));
    delete process;
    delete list;

    resetdata();

    emit processEnded(MAINPROCESS);
    return true;
}

//void Executor::pauseProcess()
//{
//    paused = true;
//}

void Executor::stopProcess()
{
    working = false;
}

void Executor::loadNormalStandardOutput()
{
    if(process->isTransactionStarted())
        emit processMessage(QString::fromStdString(process->readAll().toStdString()), NORMAL);
}

void Executor::processEndLastStatus()
{
    emit processStateUpdate(execlist.count());
}

//Think kinds of text
bool Executor::loadInfo(QList<QStringList> *list, int firstpos)
{
    QString curdata;
    QString result = "";
    curdata = list->at(firstpos).at(1);
    curdata = (curdata == "")? "(no name)" : curdata;

    // line 1
    result.append(tr("ProcessInformation : "));
    result.append(curdata);
    result.append(" : ver.");

    curdata = list->at(firstpos + 1).at(1);
    curdata = (curdata == "")? "test" : curdata;
    result.append(curdata);
    result.append(" - last update ");
    result.append(list->at(firstpos + 4).at(1));
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
    return true;
}

bool Executor::loadNormal(QList<QStringList> *list, int firstpos)
{
    int cmdc = VariantConverter::stringToInt(list->at(firstpos + 1).at(1));

    if(cmdc == 0){
        emit processMessage(tr("No executable file. Skip."), ERROR);
        return true;
    }

    //set args
    QString app;
    QStringList arguments;

    app = macroConvert(list->at(firstpos + 2).at(1));

    for(int i = 1; i < cmdc; i++){
        arguments.append(macroConvert(list->at(firstpos + 2 + i).at(1)));
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
    if(detached){
        process->startDetached(app, arguments);
    }else{
        process->start(app, arguments);
    }

    //wait commands
    if(VariantConverter::stringToBool(list->at(firstpos).at(1))){
        process->waitForStarted();
    }else{
        process->waitForFinished(-1);
    }

    return true;
}

bool Executor::loadSearch(QList<QStringList> *list, int firstpos)
{
    FileSearchLoader *loader = new FileSearchLoader();
    QStringList result = loader->searchFromXml(VariantConverter::stringToInt(list->at(firstpos).at(3)));

    emit processMessage(tr("Search : ") + QString::number(result.count()) + tr(" files found.\r\n"), SEARCH);

    //TODO : separation data detection ?
    //combine search result
    QString combineresult;
    QString sepdata = list->at(firstpos + 1).at(1);
    int cre = result.count();
    for(int i = 0; i < cre; i++){
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

    int radiodata = VariantConverter::stringToInt(list->at(firstpos + 3).at(3));

    //save to variant
    if(radiodata == 0){
        QString selectvar = list->at(firstpos + 2).at(1);
        //set data to variant
        overwriteLocalMacro(selectvar, combineresult);

    }else{
        //TODO: create data to File
        QString outputfile = list->at(firstpos + 3).at(1);

        if(outputfile != ""){
            QFile file(outputfile);

            //TODO: add overwrite permission (2017/09/02 updated)
            if(file.exists() && searchfileoverwrite){
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

bool Executor::loadScript(QList<QStringList> *list, int firstpos)
{
    QString file = list->at(firstpos + 1).at(1);
    QFileInfo info(file);
    bool result = true;

    if(info.exists()){
        //check plugin can use
        QPluginLoader loader(file);

        if(loader.load()){
            QObject *plugin = loader.instance();
            ext = qobject_cast<RunnerExtraPluginInterface *>(plugin);

//            connect(plugin, SIGNAL(sendMessage()), this, &Executor::extrafuncInternalMessage);

            int cmdc = ((QString)list->at(firstpos + 2).at(1)).toInt();

            QStringList tmp;
            for(int i = 0; i < cmdc; i++){
                tmp.append(macroConvert(list->at(firstpos + 3 + i).at(1)));
            }

            ext->setInputFileData(fileHash);
            ext->setGlobalValue(globalHash);
            ext->setLocalValue(localHash);

//            plugin->disconnect(ext, &RunnerExtraPluginInterface::sendMessage
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
    emit processStarted(OTHERPROCESS);

    //stack currentdata
    builderstack.push(pbuilder);
    localstack.push(localHash);

    //check nest
    if(builderstack.count() >= othernestmax) neststop = true;

    //alternate data
    pbuilder = new ProcessXmlBuilder();
    pbuilder->setLoadPath(list->at(firstpos + 1).at(1));

    localHash = new QHash<QString, QString>();
    setLocalList();

    //set counter
    int counter = pbuilder->count();

    //init data
    QList<QStringList> *ilist = new QList<QStringList>();
    bool checker = true;

    for(int i = 0; i < counter; i++){

        //read each list
        if(pbuilder->readItem(i, ilist)){

            //scheduler only or not
            if(VariantConverter::stringToBool(ilist->at(0).at(3)) && launchedfrom == DEFAULT) break;

            switch (getReadType(ilist->at(0).at(1))) {
            case INFO:
                checker = loadInfo(ilist, 1);
                break;

            case NORMAL:
                checker = loadNormal(ilist, 1);
                break;

            case SEARCH:
                checker = loadSearch(ilist, 1);
                break;

            case SCRIPT:
                checker = loadScript(ilist, 1);
                break;

            case OTHER:
                checker = neststop ? false : loadOther(ilist, 1);
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
    delete localHash;

    //pop data
    pbuilder = builderstack.pop();
    localHash = localstack.pop();

    //check nest
    if(builderstack.count() == 0) neststop = false;

    emit processEnded(OTHERPROCESS);

    return true;
}

bool Executor::loadTemp(QList<QStringList> *list)
{
    int selected = VariantConverter::stringToInt(list->at(1).at(1));

    switch (selected) {
    case 0/*normal*/: return loadNormal(list, pbuilder->firstPosTempNormal());

    case 1/*search*/: return loadSearch(list, pbuilder->firstPosTempSearch());

    case 2/*script*/: return loadScript(list, pbuilder->firstPosTempScript());

    case 3/*other*/:
        if(neststop){
            return false;
        }else{
            return loadOther(list, pbuilder->firstPosTempOther());
        }
    default:          return true;
    }
}

// original is part of 1 commands in one process
QString Executor::replaceInputMacro(QString original)
{
    return replaceMacro(original, &fileHash);
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
    QHash<QString, QString>::iterator i = list->find(original);
    while (i != list->end() && i.key() == original) {
        result = result.replace(original, i.value());
        ++i;
    }
    return result;
}

void Executor::overwriteLocalMacro(QString key, QString value)
{
    if(localHash->contains(key)) localHash->insert(key, value);
}

void Executor::checkExecList()
{
    int excount = execlist.count();
    int buildermax = pbuilder->count();

    if(excount == 0){
        for(int i = 0; i < buildermax; i++){
            execlist.append(i);
        }
        autoaddexec = true;

    }else{
        for(int i = 0; i < excount; i++){
            if(!(execlist.at(i) < buildermax)){
                emit processMessage(tr("Index ")
                                    + QString::number(execlist.at(i))
                                    + tr(" is not exist. \r\n"), ERROR);
                execlist.removeAt(i);
            }
        }
    }
}

int Executor::getReadType(QString type)
{
    if(type == "info") return INFO;
    if(type == "normal") return NORMAL;
    if(type == "search") return SEARCH;
    if(type == "script") return SCRIPT;
    if(type == "other") return OTHER;
    if(type == "temp") return TEMP;
    if(type == "local") return LOCAL;
    return -1;
}

void Executor::resetdata()
{
    //reset flags
    working = false;
    processfileloaded = false;
    pbuilder->setLoadPath("");
    if(autoaddexec){
        execlist.clear();
        autoaddexec = false;
    }
}

void Executor::sleep(int ms)
{
    QEventLoop loop;
    QTimer::singleShot( ms, &loop, SLOT( quit() ) );
    loop.exec();
}

int Executor::getOthernestmax() const
{
    return othernestmax;
}

void Executor::setOthernestmax(int nest)
{
    othernestmax = nest;
}

bool Executor::getSearchfileoverwrite() const
{
    return searchfileoverwrite;
}

void Executor::setSearchfileoverwrite(bool overwrite)
{
    searchfileoverwrite = overwrite;
}

int Executor::getLaunchedfrom() const
{
    return launchedfrom;
}

void Executor::setLaunchedfrom(int only)
{
    launchedfrom = only;
}
