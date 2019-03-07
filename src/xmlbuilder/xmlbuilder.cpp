#include "xmlbuilder.h"

Xmlbuilder::Xmlbuilder(QObject *)
{

}

Xmlbuilder::~Xmlbuilder()
{

}

bool Xmlbuilder::readItem(int itemid,
                                   QString firstlayername,
                                   QString attr,
                                   QList<QStringList> *itemlist)
{
    bool hasid = false;
    QString name = "";

    openFile(QFile::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError())){
        rxml->readNextStartElement();
        name = rxml->name().toString();

        if(name == firstlayername && !hasid
                && rxml->attributes().value(attr).toInt() == itemid){
            hasid = true;
        }

        if(hasid) setSearchItemData(name, itemlist);

        if(name == firstlayername && hasid
                && rxml->isEndElement()/*rxml->name()*/){
            hasid = false;
            break;
        }

    }

    checkXmlError();
    closeFile();

    //if item count is zero, return false
    return (itemlist->count() > 0)? true: false;
}

//Reading with fewer open times of opening and closing files
bool Xmlbuilder::readAllItem(QString firstlayername, QString attr, QList<QList<QStringList> *> *itemlist)
{
    bool hasid = false;
    QString name = "";
    QList<QStringList> *tmplist = nullptr;

    openFile(QFile::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError())){
        rxml->readNextStartElement();
        name = rxml->name().toString();

        if(name == firstlayername && !hasid
                && rxml->attributes().hasAttribute(attr)){
            tmplist = new QList<QStringList>();
            hasid = true;
        }

        if(hasid){
            setSearchItemData(name, tmplist);
        }

        if(name == firstlayername && hasid
                && rxml->isEndElement()){
            hasid = false;
            itemlist->append(tmplist);
        }

    }

    checkXmlError();
    closeFile();

    //if item count is zero, return false
    return (itemlist->count() > 0)? true: false;
}

void Xmlbuilder::swapItem(int beforeitemid,
                                   int afteritemid,
                                   QString rootelement,
                                   QString firstlayername,
                                   QString attr)
{
    //this function probably slower.
    QList<QStringList> before;
    QList<QStringList> after;
    readItem(beforeitemid, firstlayername, attr, &before);
    readItem(afteritemid, firstlayername, attr, &after);

    //swap items
    overwriteItem(beforeitemid, rootelement, firstlayername, attr, &after);
    overwriteItem(afteritemid, rootelement, firstlayername, attr, &before);
}

void Xmlbuilder::copyItem(int itemid,
                                   QString rootelement,
                                   QString firstlayername,
                                   QString attr,
                                   QString exchangedata){
    QList<QStringList> tmp;
    readItem(itemid, firstlayername, attr, &tmp);

    //change setting name
    QString data = tmp.at(0).at(1);
    data.append("_COPY");

    //set new title element
    QStringList elem;
    elem.append(exchangedata);
    elem.append(data);

    tmp.removeFirst();
    tmp.insert(0,elem);

    //add new item
    overwriteItem(count(firstlayername), rootelement, firstlayername, attr, &tmp);
}

bool Xmlbuilder::overwriteItem(int itemid,
                                        QString rootelement,
                                        QString firstlayername,
                                        QString attr,
                                        const QList<QStringList> *itemlist)
{
    if(count(firstlayername) < itemid){
        qDebug() << "[Xmlbuilder::overwriteItem] : counter fault : " << rootelement;
        return false;
    }

    //delete specified index
    deleteElementGroup(firstlayername, attr, itemid, true);

    //get item line end
    qint64 specifiedline;

    //check item count
    if(itemid == 0){
        specifiedline = getElementFirstLineNumber(rootelement);
    }else{
        specifiedline = getElementEndLineNumber(firstlayername, attr, (itemid - 1));
    }

    qint64 linecount = 0;

    QString forward;
    QString append = getStructureStr(itemid, firstlayername, attr, 1, itemlist, true);
    QString lest;

    //please load file readwritemode
    openFile(QFile::ReadWrite);
    openedFileReset();
    QTextStream add(file);

    //read forword data set
    while(!add.atEnd() && linecount != specifiedline){
        forward.append(add.readLine()).append(BaseXmlBuilder::endLineStr());
        linecount++;
    }

    //read lest data set
    lest = add.readAll();

    //clear filetext
    clearFileText();

    add << forward.append(append).append(lest);

    closeFile();
    return true;
}

bool Xmlbuilder::insertItem(int itemid, QString rootelement, QString firstlayername, QString attr, const QList<QStringList> *itemlist)
{
    return insertItemId(itemid, rootelement, firstlayername, attr, itemlist);
}

int Xmlbuilder::count(QString firstlayername)
{
    return getElementItemsCount(firstlayername);
}

bool Xmlbuilder::deleteItem(int itemid, QString firstlayername, QString attr)
{
    int lastindex = count(firstlayername) - 1;
    if(lastindex == itemid){
        //last item
        return deleteElementGroup(firstlayername, attr, itemid, true);
    }else{
        //get table last id
        //copy other items
        deleteItemIdRecursive(itemid, firstlayername, attr);
        //delete other items
        return deleteElementGroup(firstlayername, attr, lastindex, true);
    }
}

void Xmlbuilder::setSearchItemData(QString, QList<QStringList> *)
{
    //set search element
    return;
}

QString Xmlbuilder::getStructureStr(int itemid,
                                  QString firstlayername,
                                  QString attr,
                                  int indentnum,
                                  const QList<QStringList> *list,
                                  bool withparent)
{
    //create new tags
    QTemporaryFile tmp;
    if (tmp.open()){
        // init write devices
        //set device
        wxml->setDevice(&tmp);

        //seek specified line
        wxml->setAutoFormatting(true);

        //set 1 tabs
        wxml->setAutoFormattingIndent(-1);

        //write temp file
        if(withparent){
            writeXmlItem(itemid, firstlayername, attr, list);
        }else{
            writeElementData(list);
        }

        tmp.close();
    }

    return getTabbedXmlString(&tmp, indentnum);
}

//QString Xmlbuilder::getIdLine(int itemid, QString firstlayername, QString attr, int indentnum)
//{
//    QString line = BaseXmlBuilder::appendTabIndent(indentnum);
//    line.append("<%1 %2=\"%3\">").arg(firstlayername).arg(attr).arg(QString::number(itemid));
//    return line;
//}

qint64 Xmlbuilder::getItemFirstLine(int tablenum, QString firstlayername, QString attr)
{
    return getElementFirstLineNumber(firstlayername, attr, QString::number(tablenum));
}

void Xmlbuilder::writeXmlItem(int itemid,
                                       QString firstlayername,
                                       QString attr,
                                       const QList<QStringList> *list)
{
    wxml->writeStartElement(firstlayername);
    wxml->writeAttribute(attr, QString::number(itemid));

    writeElementData(list);

    wxml->writeEndElement();
}

void Xmlbuilder::writeElementData(const QList<QStringList> *list)
{
    //set all settings
    for(int i = 0; i < list->count(); i++){

        if(list->at(i).count() == 2){

            wxml->writeTextElement(list->at(i).at(0), list->at(i).at(1));
        }else{

            wxml->writeStartElement(list->at(i).at(0));
            for(int j = 2; j < list->at(i).count(); j+=2){
                wxml->writeAttribute(list->at(i).at(j), list->at(i).at(j+1));
            }
            wxml->writeCharacters(list->at(i).at(1));
            wxml->writeEndElement();
        }
    }
}

bool Xmlbuilder::insertItemId(int itemid
                            , QString rootelement
                            , QString firstlayername
                            , QString attr
                            , const QList<QStringList> *itemlist)
{
    int counter = count(firstlayername);
    if(counter < itemid){
        qDebug() << "[Xmlbuilder::insertItemId] : counter fault.";
        return false;
    }

    qint64 sline;

    //check item count
    if(itemid == 0){
        //add
        sline = getElementFirstLineNumber(rootelement);
    }else{
        //insert
        sline = getElementEndLineNumber(firstlayername, attr, (itemid - 1));
    }

    qint64 linecount = 0;

    QString forward;
    QString append = getStructureStr(itemid, firstlayername, attr, 1, itemlist, true);
    QString lest;

    //please load file readwritemode
    openFile(QFile::ReadWrite);
    openedFileReset();
    QTextStream add(file);

    //read forword data set
    while(linecount < sline){
        forward.append(add.readLine()).append(BaseXmlBuilder::endLineStr());
        linecount++;
    }

    //read lest data set
    //update dataset
    QString curline;
    QString tmp;
    QRegularExpression re("<\\w+( \\w+=\"(\\w+)\")");
    int newid = itemid;

    while(!add.atEnd()){
        curline = add.readLine();

        if(curline.contains(re)){
            newid++;
            lest.append(curline.replace(re, QString("<%1 %2=\"%3\"").arg(firstlayername).arg(attr).arg(newid)));
        }else{
            lest.append(curline);
        }

        lest.append(BaseXmlBuilder::endLineStr());
    }

    //clear filetext
    clearFileText();

    add << forward.append(append).append(lest);

    closeFile();
    return true;
}

bool Xmlbuilder::deleteItemId(int destitemid,
                                       int fromitemid,
                                       QString firstlayername,
                                       QString attr)
{
    int counter = count(firstlayername);
    if(counter < fromitemid || counter < destitemid){
        qDebug() << "[Xmlbuilder::shiftXmlData] : counter fault.";
        return false;
    }

    //delete specified index
    deleteElementGroup(firstlayername, attr, destitemid, false);

    //get item line start
    qint64 specifiedline = getItemFirstLine(destitemid, firstlayername, attr);
    int linecount = 0;

    QString forward;
    QList<QStringList> data;
    readItem(fromitemid, firstlayername, attr, &data);
    QString append = getStructureStr(destitemid, firstlayername, attr, 2, &data, false);
    QString lest;

    //please load file readwritemode
    openFile(QFile::ReadWrite);
    openedFileReset();

    //set file to stream
    QTextStream add(file);

    //read forword data set
    while(!add.atEnd() && linecount != specifiedline){
        forward.append(add.readLine());
        forward.append(BaseXmlBuilder::endLineStr());
        linecount++;
    }

    //read lest data set
    lest = add.readAll();

    //clear filetext
    clearFileText();

    add << forward.append(append).append(lest);

    closeFile();
    return true;
}

bool Xmlbuilder::deleteItemIdRecursive(int deleteitemid,
                                                QString firstlayername,
                                                QString attr)
{
    //get tableid
    int tableid = count(firstlayername);

    if(tableid < deleteitemid){
        qDebug() << "[Xmlbuilder::shiftXmlDataRecursive] : counter fault.";
        return false;
    }

    for(int i = deleteitemid; i < tableid; i++){
        deleteItemId(i, i+1, firstlayername, attr);
    }
    return true;
}
