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
    readFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
//        rxml->readNext();
        rxml->readNextStartElement();

//        if (rxml->isStartElement())
//        {
//            QString name = rxml->name().toString();

//            if(name == firstlayername && !hasid)
//            {
//                QString val = rxml->attributes().value(attr).toString();

//                if(val == QString::number(itemid)) hasid = true;
//            }
        name = rxml->name().toString();

        if(name == firstlayername && !hasid
                && rxml->attributes().value(attr).toInt() == itemid)
        {
            hasid = true;
        }


        if(hasid) setSearchItemData(name, itemlist);
//        }

        if(name == firstlayername && hasid
                && rxml->isEndElement()/*rxml->name()*/)
        {
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
    QList<QStringList> *tmplist;

    openFile(QFile::ReadOnly);
    readFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
        rxml->readNextStartElement();
        name = rxml->name().toString();

        if(name == firstlayername && !hasid
                && rxml->attributes().hasAttribute(attr))
        {
            tmplist = new QList<QStringList>();
            hasid = true;
        }

        if(hasid){
            setSearchItemData(name, tmplist);
        }

        if(name == firstlayername && hasid
                && rxml->isEndElement())
        {
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
                                   QString exchangedata)
{
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
        qDebug() << "overwriteItem() : counter fault. :" << rootelement;
        return false;
    }

    //delete specified index
    deleteSpecifiedElementGroup(firstlayername, attr, itemid, true);

    //get item line end
    int specifiedline;

    //check item count
    if(itemid == 0){
        specifiedline = getSpecifiedElementLineFirst(rootelement);
    }else{
        specifiedline = getSpecifiedElementLineEnd(firstlayername, attr, (itemid - 1));
    }

    int linecount = 0;

    QString forward;
    QString append = getAppendData(itemid, firstlayername, attr, 1, itemlist, true);
    QString lest;

    //please load file readwritemode
    openFile(QFile::ReadWrite);
    readFileReset();
    QTextStream add(file);

    //read forword data set
    while(!add.atEnd() && linecount != specifiedline){
        //WARNING:hard coding
        //Windows
#ifdef Q_OS_WIN
        forward.append(add.readLine()).append("\n");
#else
        forward.append(add.readLine()).append("\r\n");
#endif
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

int Xmlbuilder::count(QString firstlayername)
{
    return getSpecifiedElementItemsCount(firstlayername);
}

bool Xmlbuilder::deleteItem(int itemid, QString firstlayername, QString attr)
{
    int lastindex = count(firstlayername) - 1;
    if(lastindex == itemid){
        //last item
        return deleteSpecifiedElementGroup(firstlayername, attr, itemid, true);
    }else{
        //get table last id
        //copy other items
        shiftXmlDataRecursive(itemid, firstlayername, attr);
        //delete other items
        return deleteSpecifiedElementGroup(firstlayername, attr, lastindex, true);
    }
}

void Xmlbuilder::setSearchItemData(QString, QList<QStringList> *)
{
    //set search element
    return;
}

QString Xmlbuilder::getAppendData(int itemid,
                                           QString firstlayername,
                                           QString attr,
                                           int indentnum,
                                           const QList<QStringList> *list,
                                           bool withparent)
{
    //create new tags
    QTemporaryFile tmp;
    if (tmp.open())
    {
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

    return getAdjustedXmlDataString(&tmp, indentnum);
}

int Xmlbuilder::getItemFirstLine(int tablenum, QString firstlayername, QString attr)
{
    return getSpecifiedElementLineFirst(firstlayername, attr, QString::number(tablenum));
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

bool Xmlbuilder::shiftXmlData(int destitemid,
                                       int fromitemid,
                                       QString firstlayername,
                                       QString attr)
{
    int counter = count(firstlayername);
    if(counter < fromitemid || counter < destitemid){
        qDebug() << "shiftXmlData() : counter fault.";
        return false;
    }

    //delete specified index
    deleteSpecifiedElementGroup(firstlayername, attr, destitemid, false);

    //get item line start
    int specifiedline = getItemFirstLine(destitemid, firstlayername, attr);
    int linecount = 0;

    QString forward;
    QList<QStringList> data;
    readItem(fromitemid, firstlayername, attr, &data);
    QString append = getAppendData(destitemid, firstlayername, attr, 2, &data, false);
    QString lest;

    //please load file readwritemode
    openFile(QFile::ReadWrite);
    readFileReset();

    //set file to stream
    QTextStream add(file);

    //read forword data set
    while(!add.atEnd() && linecount != specifiedline){
        forward.append(add.readLine());
        //WARNING:hard coding
        //Windows
#ifdef Q_OS_WIN
        forward.append("\n");
#else
        forward.append("\r\n");
#endif
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

bool Xmlbuilder::shiftXmlDataRecursive(int deleteitemid,
                                                QString firstlayername,
                                                QString attr)
{
    //get tableid
    int tableid = count(firstlayername);

    if(tableid < deleteitemid){
        qDebug() << "shiftXmlDataRecursive() : counter fault.";
        return false;
    }

    for(int i = deleteitemid; i < tableid; i++){
        shiftXmlData(i, i+1, firstlayername, attr);
    }
    return true;
}
