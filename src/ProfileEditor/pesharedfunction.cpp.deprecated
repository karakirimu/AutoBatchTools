#include "pesharedfunction.h"

#include <QSettings>

PESharedFunction::PESharedFunction(QObject *parent) : QObject(parent)
{
    //set new xml builder
    builder = new ProcessXmlBuilder();

    //init filename
    tmpbku = "";
    loadedfile = "";

    //load temp path
    QSettings settings( "./psettings.ini", QSettings::IniFormat );

    settings.beginGroup("BASICSETTING");
    tmpfilepath = settings.value("TEMPDIR", "./").toString();
    settings.endGroup();
}

PESharedFunction::~PESharedFunction()
{
    //TODO:
    if(tmpbku != ""){
        QFile file(tmpfilepath + tmpbku);
        if(!file.remove()){
            qDebug() << file.errorString() << file.isOpen();

        }
    }

//    builder->delLoadPath();
    delete builder;
}

void PESharedFunction::generateNewFile()
{
    if(tmpbku != ""){
        QFile::remove(tmpfilepath + tmpbku);
    }

    //set temp path
    tmpbku = ".~$New_File_" /*+ QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")*/;
    loadedfile = "";
    builder->setLoadPath(tmpbku);

    emit fileNameChanged("New_File", tmpbku);
}

void PESharedFunction::loadFile(QString filename)
{
    if(tmpbku != ""){
        QFile::remove(tmpfilepath + tmpbku);
    }

    //set temp path
    QFileInfo info(filename);
    tmpfilepath = info.canonicalPath();
    tmpbku = "/.~$" + info.fileName();
    loadedfile = filename;
    QFile::copy(loadedfile, tmpfilepath + tmpbku);
    builder->setLoadPath(tmpfilepath + tmpbku);

    emit fileNameChanged(filename, tmpbku);
}

bool PESharedFunction::readItem(int itemid, QList<QStringList> *itemlist)
{
    qDebug() << "PEShared:readItem : " << itemid;
    if(itemid > -1) return builder->readItem(itemid, itemlist);

    return false;
}

//DEPENDS_XML
void PESharedFunction::addItem()
{
    //initiate data
    QList<QStringList> newlist;
    newlist.append((QStringList() << "type" << "normal" << "only" << "no"));
    newlist.append((QStringList() << "timeout" << "no" << "dur" << "30000"));
    newlist.append((QStringList() << "cmdc" << "0"));
    builder->addItem(&newlist);
}

void PESharedFunction::deleteItem(int current)
{
    if(current < 1 || this->tmpbku == loadedfile) return;
    builder->deleteItem(current);
}

void PESharedFunction::editItem(int itemid, QList<QStringList> *itemlist)
{
    if(this->tmpbku == loadedfile) return;
    builder->editItem(itemid, itemlist);
}

void PESharedFunction::copyItem(int current)
{
    if(current < 1 || this->tmpbku == loadedfile) return;
    builder->copyItem(current);
}

void PESharedFunction::upItem(int current)
{
    //hard coding
    if(this->tmpbku == loadedfile || current < 3) return;
    builder->swapItem(current, current-1);
}

void PESharedFunction::downItem(int current)
{
    if(this->tmpbku == loadedfile
            || builder->count() == current+1
            || current < 2) return;
    builder->swapItem(current, current+1);
}

QString PESharedFunction::openEditAction(QWidget *parent)
{
    QString fileName =
            QFileDialog::getOpenFileName(parent, tr("Open Edit file"),\
                                                    QDir::currentPath(),\
                                                    tr("Profile (*.xml *.apro)"));
    if (fileName.isEmpty()) return "";

    loadFile(fileName);

    QFileInfo info(fileName);
    return info.fileName();
}

QString PESharedFunction::saveEditAction(QWidget *parent)
{
    if(this->tmpbku == loadedfile) return "";

    QString fileName =
            QFileDialog::getSaveFileName(parent, tr("Save Edit file"),\
                                         QDir::currentPath(),\
                                         tr("APRO Files (*.apro)"));
    return saveAction(fileName);
}

QString PESharedFunction::saveEditOverWriteAction(QWidget *parent)
{
    if(this->tmpbku == loadedfile) return "";

    if(loadedfile == ""){
        return saveEditAction(parent);
    }else{
        //delete existed files
        if(QFile::exists(loadedfile)){
            QFile::remove(loadedfile);
        }
        QFile::copy(tmpbku, loadedfile);

        //window title change signal
        QFileInfo info(loadedfile);
        return info.fileName();
    }
}

QString PESharedFunction::exportData(QWidget *parent)
{
    if(this->tmpbku == loadedfile) return "";

    QString fileName =
            QFileDialog::getSaveFileName(parent, tr("Export XML file"),\
                                         QDir::currentPath(),\
                                         tr("XML Files (*.xml)"));

    //set new xmlbuilder
    ProcessXmlBuilder *xbuilder = new ProcessXmlBuilder();

    //set temp new itemlist
    QList<QStringList> *itemlist = new QList<QStringList>();

    //delete file is existed
    QFile file(fileName);
    if(file.exists()){
        file.remove();
    }

    xbuilder->setLoadPath(fileName);
    xbuilder->deleteItem(0);
    xbuilder->deleteItem(1);

    //get default builder count
    int def_counter = itemCount();
    for(int i = 0; i < def_counter; i++){
        //read old item
        xbuilder->readItem(i, itemlist);

        //qdebug
        qDebug() << "peshared::export count" << i;

        //convert temp
        convertToDefaultList(itemlist);
        xbuilder->addItem(itemlist);

        //clear temp itemlist
        itemlist->clear();
    }

    delete itemlist;
    delete xbuilder;
    return fileName;
}

int PESharedFunction::itemCount()
{
    // no file existed
    if(this->tmpbku == loadedfile) return 0;
    return builder->count();
}

///DEPENDS_XML
void PESharedFunction::createInfoList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "info"));
    newlist->append((QStringList() << "name" << list->at(0)));
    newlist->append((QStringList() << "ver" << list->at(1)));
    newlist->append((QStringList() << "author" << list->at(2)));
    newlist->append((QStringList() << "desc" << list->at(3)));
    newlist->append((QStringList() << "uptime" << QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss")));
}

///DEPENDS_XML
void PESharedFunction::createLocalList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = VariantConverter::stringToInt(list->at(0)) * 2;
    newlist->append((QStringList() << "type" << "local"));
    newlist->append((QStringList() << "localc" << list->at(0)));
    for(int i = 0; i < rcount; i+=2){
        newlist->append((QStringList() << "lvar"
                         << list->at(i + 1) << "lval" << list->at(i + 2)));
    }
}

///DEPENDS_XML
void PESharedFunction::createNormalList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = VariantConverter::stringToInt(list->at(3));
    newlist->append((QStringList() << "type" << "normal" << "only" << list->at(0)));
    newlist->append((QStringList() << "timeout" << list->at(1) << "dur" << list->at(2)));
    newlist->append((QStringList() << "cmdc" << list->at(3)));
    for(int i = 0; i < rcount; i++){
        newlist->append((QStringList() << "cmd"
                         << list->at(i + 4) << "id" << QString::number(i)));
    }
}

///DEPENDS_XML
void PESharedFunction::createSearchList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "search" << "only" << list->at(0)));
    newlist->append((QStringList() << "sname" << list->at(1) << "id" << list->at(2)));
    newlist->append((QStringList() << "sep" << list->at(3)));
    newlist->append((QStringList() << "var" << list->at(4)));
    newlist->append((QStringList() << "output" << list->at(5) << "radio" << list->at(6)));
}

///DEPENDS_XML
void PESharedFunction::createScriptList(QList<QStringList> *newlist, QStringList *list)
{
    int rcount = VariantConverter::stringToInt(list->at(3));
    newlist->append((QStringList() << "type" << "script" << "only" << list->at(0)));
    newlist->append((QStringList() << "name" << list->at(1)));
    newlist->append((QStringList() << "file" << list->at(2)));
    newlist->append((QStringList() << "cmdc" << list->at(3)));
    for(int i = 0; i < rcount; i++){
        newlist->append((QStringList() << "cmd"
                         << list->at(i + 4) << "id" << QString::number(i)));
    }
}

///DEPENDS_XML
void PESharedFunction::createOtherList(QList<QStringList> *newlist, QStringList *list)
{
    newlist->append((QStringList() << "type" << "other" << "only" << list->at(0)));
    newlist->append((QStringList() << "name" << list->at(1)));
    newlist->append((QStringList() << "file" << list->at(2)));
}

///DEPENDS_XML
void PESharedFunction::convertToDefaultList(QList<QStringList> *temp)
{
    QString data = temp->at(0).at(1);

    if(data != "temp") return;

    QStringList list;

    //get current position of temporary list. (gui part)
    int istack = VariantConverter::stringToInt(temp->at(1).at(1));

    //!Warning HARD CODING
    int count;
    const int constdata[4] = {firstPosNormal(),
                              firstPosSearch(),
                              firstPosScript(),
                              firstPosOther()};

    int firstpos = constdata[istack];
    int skippos = 0;

    if(istack == 2)
        skippos += VariantConverter::stringToInt(temp->at(firstPosNormal() + 1).at(1));

    //shift counts
    firstpos += skippos;

    //add only list
    list.append(temp->at(0).at(3));

    switch(istack){
    case 0:
        //Normal
        list.append(temp->at(firstpos).at(1));
        list.append(temp->at(firstpos + 1).at(1));
        count = VariantConverter::stringToInt(temp->at(firstpos + 1).at(1));
        for(int i = 0; i < count; i++){
            list.append(temp->at(firstpos + 2 + i).at(1));
        }
        temp->clear();
        createNormalList(temp, &list);
        break;
    case 1:
        //Search
        list.append(temp->at(firstpos).at(1));
        list.append(temp->at(firstpos).at(3));
        list.append(temp->at(firstpos + 1).at(1));
        list.append(temp->at(firstpos + 2).at(1));
        list.append(temp->at(firstpos + 3).at(1));
        list.append(temp->at(firstpos + 3).at(3));
        temp->clear();
        createSearchList(temp, &list);
        break;
    case 2:
        //Script
        list.append(temp->at(firstpos).at(1));
        list.append(temp->at(firstpos + 1).at(1));
        list.append(temp->at(firstpos + 2).at(1));
        count = VariantConverter::stringToInt(temp->at(firstpos + 2).at(1));
        for(int i = 0; i < count; i++){
            list.append(temp->at(firstpos + 3 + i).at(1));
        }
        temp->clear();
        createScriptList(temp, &list);
        break;
    case 3:
        //Other
        list.append(temp->at(firstpos).at(1));
        list.append(temp->at(firstpos + 1).at(1));
        temp->clear();
        createOtherList(temp, &list);
        break;
    default:
        break;
    }

}

int PESharedFunction::checkFileEdited()
{
    ConvertChecker cc;
    return cc.checkMPxmlDiff(tmpbku, loadedfile);
}

int PESharedFunction::setInformationType(QString type)
{
    if(type == "info") return 0;
    if(type == "normal") return 1;
    if(type == "search") return 2;
    if(type == "script") return 3;
    if(type == "other") return 4;
    if(type == "temp") return 5;
    return -1;
}

int PESharedFunction::firstPosNormal() const
{
    return builder->firstPosTempNormal();
}

int PESharedFunction::firstPosSearch() const
{
    return builder->firstPosTempSearch();
}

int PESharedFunction::firstPosScript() const
{
    return builder->firstPosTempScript();
}

int PESharedFunction::firstPosOther() const
{
    return builder->firstPosTempOther();
}

QString PESharedFunction::saveAction(QString fileName)
{
    if (fileName.isEmpty()) return "";

    //delete existed files
    if(QFile::exists(fileName)){
        QFile::remove(fileName);
    }

    QFile::copy(tmpbku, fileName);
    QFile::remove(tmpbku);

    loadFile(fileName);

    QFileInfo info(fileName);
    return info.fileName();
}
