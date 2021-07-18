/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "profilexmlbuilder.h"

ProfileXmlBuilder::ProfileXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/profiles");

    if(count() == 0){
        createDocument();
    }
}

ProfileXmlBuilder::~ProfileXmlBuilder()
{

}

bool ProfileXmlBuilder::readAll(QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[ProfileXmlBuilder::readAll]";
    return Xmlbuilder::readAllItem(FIRSTLAYER
                                   , ATTR
                                   , itemlist);
}

bool ProfileXmlBuilder::writeAll(const QList<QList<QStringList> *> *itemlist)
{
    qDebug() << "[ProfileXmlBuilder::writeAll]";
    return Xmlbuilder::writeAllItem(ROOTELEMENT
                                    , ATTRVERSION
                                    , VERSION
                                    , FIRSTLAYER
                                    , ATTR
                                    , itemlist);
}

bool ProfileXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool ProfileXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count()
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

bool ProfileXmlBuilder::insertItem(int itemid
                                   , QList<QStringList> *itemlist)
{
    return Xmlbuilder::insertItem(itemid
                                  , ROOTELEMENT
                                  , FIRSTLAYER
                                  , ATTR
                                  , itemlist);
}

bool ProfileXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool ProfileXmlBuilder::editItem(int itemid
                                 , const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , ROOTELEMENT
                                     , FIRSTLAYER
                                     , ATTR
                                     , itemlist);
}

void ProfileXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid
                         , afteritemid
                         , ROOTELEMENT
                         , FIRSTLAYER
                         , ATTR);
}

void ProfileXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT, ATTRVERSION, VERSION);
}

int ProfileXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void ProfileXmlBuilder::createVarElement(QList<QStringList> *internal
                                         , QStringList *var)
{
    if(internal->count() > 0){
        qDebug() << "[ProfileXmlBuilder::createVarElement] "
                 << "The first argument 'internal' needs empty list.";
        return;
    }

    if(var->count() != 3){
        qDebug() << "[ProfileXmlBuilder::createVarElement] "
                 << "The second argument 'var' needs 3 data. name, desc and file.";
        return;
    }

    internal->append(QStringList() << PROFILE_NAME << var->at(0));
    internal->append(QStringList() << PROFILE_DESCRIPTION << var->at(1));
    internal->append(QStringList() << PROFILE_FILE << var->at(2));
}

void ProfileXmlBuilder::setSearchItemData(QString element
                                          , QList<QStringList> *list)
{
    if(element == PROFILE_NAME
            || element == PROFILE_DESCRIPTION
            || element == PROFILE_FILE)
    {
        list->append(QStringList() << element << rxml->readElementText());
    }
}
