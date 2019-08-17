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

#include "searchxmlbuilder.h"

SearchXmlBuilder::SearchXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/search");

    if(count() == 0){
        createDocument();
    }
}

SearchXmlBuilder::~SearchXmlBuilder()
{
}

bool SearchXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool SearchXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool SearchXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool SearchXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void SearchXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

// this function needs to call setrowcount
void SearchXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, SEARCH_NAME);
}

bool SearchXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void SearchXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int SearchXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

QString SearchXmlBuilder::fetch(QString tag, QString attr, const QList<QStringList> *loadbase)
{
    int count = loadbase->count();
    int i = 0;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == SEARCH_NONE) return loadbase->at(i).at(1);

            listnummax = loadbase->at(i).count();
            if(listnummax > 3 && attr == loadbase->at(i).at(2)) return loadbase->at(i).at(3);
            if(listnummax > 5 && attr == loadbase->at(i).at(4)) return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return SEARCH_NONE;
}

/**
 * @brief SearchXmlBuilder::getItemData
 * @param element
 * @param list
 * @details
 * QList<QStringList> structures
 * QList
 * |
 * +[0] QStringList <- inserted order as follows. [elementname, value, attr1, attr1_value, attr2,...]
 * +[1] QStringList
 *
 * this functions getting set element child only.
 */
void SearchXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == SEARCH_NAME
            || element == SEARCH_KEYWORD
            || element == SEARCH_DIR
            || element == SEARCH_RECURSIVE)
    {
        //add element and text
        list->append(QStringList() << element << rxml->readElementText());
    }

    if(element == SEARCH_SECONDS
            || element == SEARCH_REGEX)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ENABLED << rxml->attributes().value(ENABLED).toString());
    }

    if(element == SEARCH_FSIZE_1
            || element == SEARCH_FSIZE_2
            || element == SEARCH_CREATION
            || element == SEARCH_MODIFIED)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << ENABLED << rxml->attributes().value(ENABLED).toString()
                     << COMBO << rxml->attributes().value(COMBO).toString());
    }

    //qDebug() << "//" << element << "//";
}
