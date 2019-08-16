/*
 * Copyright 2016-2019 karakirimu
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

#include "xmlbuilder.h"

Xmlbuilder::Xmlbuilder(QObject *)
{

}

Xmlbuilder::~Xmlbuilder()
{

}

/**
 * @fn Xmlbuilder::readItem
 * @brief If there is an attribute with the specified
 *        itemid in firstlayername, data is assigned to itemlist.
 *
 * @param itemid         : Value in attribute
 * @param firstlayername : firstlayer name
 * @param attr           : attribute
 * @param itemlist       : Instance of data to get
 *
 * @return Whether data acquisition was successful.
 */
bool Xmlbuilder::readItem(int itemid, QString firstlayername,
                          QString attr, QList<QStringList> *itemlist)
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

/**
 * @fn Xmlbuilder::readAllItem
 * @brief All the data when there is an itemid associated with attr is
 *        extracted and stored in itemlist.
 *
 * @param firstlayername : firstlayer name
 * @param attr           : attribute name
 * @param itemlist       : Instance of data to get
 *
 * @return if item count is zero, return false.
 * @remarks Reading with fewer open times of opening and closing files.
 */
bool Xmlbuilder::readAllItem(QString firstlayername, QString attr,
                             QList<QList<QStringList> *> *itemlist)
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

    return (itemlist->count() > 0)? true: false;
}

/**
 * @fn Xmlbuilder::swapItem
 * @brief Exchange tags in the following layers within the selected itemid.
 *
 * @param beforeitemid   : The value of the attribute you want to exchange.
 * @param afteritemid    : Attribute value after exchange.
 * @param rootelement    : Root tag name.
 * @param firstlayername : firstlayer name
 * @param attr           : attribute name.
 */
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

/**
 * @deprecated It is better not to use.
 *
 * @fn Xmlbuilder::copyItem
 * @brief Copy the layer below the itemid attribute and insert it into the next itemid.
 *
 * @param itemid         : The value of the attribute you want to copy.
 * @param rootelement    : Root tag name.
 * @param firstlayername : firstlayer name
 * @param attr           : attribute name.
 * @param exchangedata   : ?
 */
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

/**
 * @fn Xmlbuilder::overwriteItem
 * @brief The attribute tag containing the specified itemid is overwritten with
 *        the information described in the itemlist.
 *
 * @param itemid         : The value of the attribute you want to exchange.
 * @param rootelement    : Root tag name.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 * @param itemlist       : An instance of data to set.
 *
 * @return Whether writing was successful.
 */
bool Xmlbuilder::overwriteItem(int itemid,
                               QString rootelement,
                               QString firstlayername,
                               QString attr,
                               const QList<QStringList> *itemlist)
{
    if(count(firstlayername) < itemid){
        qDebug() << "[Xmlbuilder::overwriteItem] : No Itemid : " << rootelement;
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

/**
 * @fn Xmlbuilder::count
 * @brief returns the number of tag blocks contained in firstlayername.
 *
 * @param firstlayername : firstlayer tag name.
 *
 * @return The number of tag blocks included in the tag.
 */
int Xmlbuilder::count(QString firstlayername)
{
    return getElementItemsCount(firstlayername);
}

/**
 * @fn Xmlbuilder::deleteItem
 * @brief Deletes the element in the specified firstlayername.
 *
 * @param itemid         : The value of the attribute you want to exchange.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 *
 * @return Whether or not itemid with attr attribute in firstlayername tag
 *         could be deleted.
 */
bool Xmlbuilder::deleteItem(int itemid, QString firstlayername, QString attr)
{
    int lastindex = count(firstlayername) - 1;
    if(lastindex == itemid){
        //last item
        return deleteElementGroup(firstlayername, attr, itemid, true);
    }else{
        //get table last id
        //copy other items
        deleteItemIdToEnd(itemid, firstlayername, attr);
        //delete other items
        return deleteElementGroup(firstlayername, attr, lastindex, true);
    }
}

/**
 * @fn Xmlbuilder::setSearchItemData
 * @brief Implements a method of assigning a value to QStringList in QList
 *        used when retrieving the specified element.
 *
 * @param element : An element that requires an assignment method.
 * @param list    : A list where items are set. (same as itemlist)
 */
void Xmlbuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    Q_UNUSED(element);
    Q_UNUSED(*list);
    //set search element
    return;
}

/**
 * @fn Xmlbuilder::getStructureStr
 * @brief This function starts with firstlayername and returns the entire
 *        string in an xml block (e.g. <abc> </ abc>) containing the block
 *        defined in list. firstlayername can define the initial value of
 *        indent with indentnum. Blocks below it are automatically indented.
 *
 * @param itemid         : The value of the attribute you want to exchange.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 * @param indentnum      : Number of tabs to add.
 * @param list           : A list where items are set. (same as itemlist)
 * @param withparent     : Whether to generate an attribute tag of itemid.
 *
 * @return XML string with properly entered tabs and newlines.
 */
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

/**
 * @fn Xmlbuilder::getItemFirstLine
 * @brief A wrapper for a function that returns the first row of the tag
 *        block for the element whose value associated with attr matches.
 *
 * @param tablenum       : Value associated with the attribute.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 *
 * @return The first line of the tag block specified by element.
 *         If it does not have any elements, then returns -1.
 */
qint64 Xmlbuilder::getItemFirstLine(int tablenum, QString firstlayername, QString attr)
{
    return getElementFirstLineNumber(firstlayername, attr, QString::number(tablenum));
}

/**
 * @fn Xmlbuilder::writeXmlItem
 * @brief Write the elements in list to an XML file.
 *
 * @param itemid         : The value of the attribute you want to exchange.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 * @param list           : A list where items are set. (same as itemlist)
 */
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

/**
 * @fn Xmlbuilder::writeElementData
 * @brief Write xml file.
 *
 * @param list       : An instance of data to set.
 */
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

/**
 * @fn Xmlbuilder::insertItemId
 * @brief Inserts the data of itemid with attr attribute in firstlayername.
 *
 * @param itemid         : The value of the attribute you want to exchange.
 * @param rootelement    : Root tag name.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 * @param itemlist       : An instance of data to set.
 *
 * @return Whether it was successfully inserted into the XML file.
 */
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

/**
 * @fn Xmlbuilder::deleteItemId
 * @brief Delete the block with the attribute with the specified id.
 *        If there is an attribute block with the specified id after
 *        the deleted block, the id value is overwritten as destitemid.
 *
 * @param destitemid     : Id of the attribute block with the specified id you want to delete.
 * @param fromitemid     : Attribute block with id overwritten as destitemid.
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 *
 * @return Whether it was successfully deleted into the XML file.
 */
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

/**
 * @fn Xmlbuilder::deleteItemIdToEnd
 * @brief Deletes from the attribute with deleteitemid value to the last id with
 *        this attribute.
 *
 * @param deleteitemid   : the value to delete associated with the attr attribute
 * @param firstlayername : firstlayer tag name.
 * @param attr           : attribute name.
 *
 * @return Whether the specified element could be deleted.
 */
bool Xmlbuilder::deleteItemIdToEnd(int deleteitemid,
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
