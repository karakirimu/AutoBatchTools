/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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

bool SearchXmlBuilder::readAll(QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[SearchXmlBuilder::readAll]";
    return Xmlbuilder::readAllItem(FIRSTLAYER
                                   , ATTR
                                   , itemlist);
}

bool SearchXmlBuilder::writeAll(const QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[SearchXmlBuilder::writeAll]";
    return Xmlbuilder::writeAllItem(ROOTELEMENT
                                    , ATTRVERSION
                                    , VERSION
                                    , FIRSTLAYER
                                    , ATTR
                                    , itemlist);
}

bool SearchXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid
                                , FIRSTLAYER
                                , ATTR
                                , itemlist);
}

bool SearchXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count()
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

bool SearchXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool SearchXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

void SearchXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid
                         , afteritemid
                         , ROOTELEMENT
                         , FIRSTLAYER
                         , ATTR);
}

// this function needs to call setrowcount
void SearchXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, SEARCH_NAME);
}

void SearchXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT, ATTRVERSION, VERSION);
}

int SearchXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

QString SearchXmlBuilder::fetch(QString tag,
                                QString attr,
                                const QList<QStringList> *loadbase)
{
    qsizetype count = loadbase->count();
    int i = 0;
    qsizetype listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == SEARCH_NONE) return loadbase->at(i).at(1);

            listnummax = loadbase->at(i).count();

            if(listnummax > 3 && attr == loadbase->at(i).at(2))
                return loadbase->at(i).at(3);

            if(listnummax > 5 && attr == loadbase->at(i).at(4))
                return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return SEARCH_NONE;
}

/**
 * @brief SearchXmlBuilder::getItemData
 * @details
 * QList<QStringList> structures
 * QList
 * |
 * +[0] QStringList <- inserted order as follows. [elementname, value, attr1, attr1_value, attr2,...]
 * +[1] QStringList
 *
 * this functions getting set element child only.
 */
void SearchXmlBuilder::setSearchItemData(QString element
                                         , QList<QStringList> *list)
{
    if(element == SEARCH_NAME
        || element == SEARCH_KEYWORD
        || element == SEARCH_DIR
        || element == SEARCH_RECURSIVE
        || element == SEARCH_SECONDS
        || element == SEARCH_REGEX
        || element == SEARCH_FSIZE_1
        || element == SEARCH_FSIZE_2
        || element == SEARCH_CREATION
        || element == SEARCH_MODIFIED) {

        QStringList read;
        QString attrname = "";

        for(auto &attr : rxml->attributes()){
            attrname = attr.name().toString();
            if(attrname == ENABLED
                || attrname == COMBO){
                read.append(attrname);
                read.append(attr.value().toString());
            }
        }

        read.prepend(rxml->readElementText());
        read.prepend(element);
        list->append(read);
    }
}
