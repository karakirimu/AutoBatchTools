/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "stringxmlbuilder.h"

StringXmlBuilder::StringXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/string");

    if(count() == 0){
        createDocument();
    }
}

StringXmlBuilder::~StringXmlBuilder()
{
}

bool StringXmlBuilder::readAll(QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[StringXmlBuilder::readAll]";
    return Xmlbuilder::readAllItem(FIRSTLAYER
                                   , ATTR
                                   , itemlist);
}

bool StringXmlBuilder::writeAll(const QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[ProcessXmlBuilder::writeAll]";
    return Xmlbuilder::writeAllItem(ROOTELEMENT
                                    , ATTRVERSION
                                    , VERSION
                                    , FIRSTLAYER
                                    , ATTR
                                    , itemlist);
}

bool StringXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool StringXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count()
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

bool StringXmlBuilder::insertItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::insertItem(itemid
                                  , ROOTELEMENT
                                  , FIRSTLAYER
                                  , ATTR
                                  , itemlist);
}

bool StringXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool StringXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

void StringXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid
                         , afteritemid
                         , ROOTELEMENT
                         , FIRSTLAYER
                         , ATTR);
}

void StringXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT, ATTRVERSION, VERSION);
}

int StringXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void StringXmlBuilder::createVarElement(QList<QStringList> *internal
                                        , QStringList *var)
{
    if(internal->count() > 0){
        qDebug() << "[StringXmlBuilder::createVarElement] "
                 << "The first argument 'internal' needs empty list.";
        return;
    }

    if(var->count() != 2){
        qDebug() << "[StringXmlBuilder::createVarElement] "
                 << "The second argument 'var' needs 2 data. variable and value.";
        return;
    }

    internal->append(QStringList() << STRING_VARIANT << var->at(0));
    internal->append(QStringList() << STRING_VALUE   << var->at(1));
}

void StringXmlBuilder::setSearchItemData(QString element
                                         , QList<QStringList> *list)
{
    if(element == STRING_VARIANT
            || element == STRING_VALUE)
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
