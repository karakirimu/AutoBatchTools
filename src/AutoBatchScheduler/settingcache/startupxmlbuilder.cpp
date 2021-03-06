/*
 * Copyright 2016-2021 karakirimu
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

#include "startupxmlbuilder.h"

StartupXmlBuilder::StartupXmlBuilder(QObject *)
{
    //set new folder
    QDir dir;
    dir.mkdir("./settings");

    //set new file
    setFileName("./settings/startup");

    if(count() == 0){
        createDocument();
    }
}

StartupXmlBuilder::~StartupXmlBuilder()
{
}

bool StartupXmlBuilder::readAll(QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[StartupXmlBuilder::readAll]";
#endif
    return Xmlbuilder::readAllItem(sxc.TAG_FIRSTLAYER
                                   , sxc.ATTR_ITEM_ID_INT
                                   , itemlist);
}

bool StartupXmlBuilder::writeAll(const QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[StartupXmlBuilder::writeAll]";
#endif
    return Xmlbuilder::writeAllItem(sxc.TAG_ROOT
                                    , sxc.ATTR_ROOT
                                    , sxc.ATTRVALUE_ROOT
                                    , sxc.TAG_FIRSTLAYER
                                    , sxc.ATTR_ITEM_ID_INT
                                    , itemlist);
}

bool StartupXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid
                                , sxc.TAG_FIRSTLAYER
                                , sxc.ATTR_ITEM_ID_INT
                                , itemlist);
}

bool StartupXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count()
                                    , sxc.TAG_ROOT
                                    , sxc.TAG_FIRSTLAYER
                                    , sxc.ATTR_ITEM_ID_INT
                                    , itemlist);
}

bool StartupXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid
                                  , sxc.TAG_FIRSTLAYER
                                  , sxc.ATTR_ITEM_ID_INT);
}

bool StartupXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , sxc.TAG_ROOT
                                     , sxc.TAG_FIRSTLAYER
                                     , sxc.ATTR_ITEM_ID_INT
                                     , itemlist);
}

void StartupXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid
                         , afteritemid
                         , sxc.TAG_ROOT
                         , sxc.TAG_FIRSTLAYER
                         , sxc.ATTR_ITEM_ID_INT);
}

void StartupXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid
                         , sxc.TAG_ROOT
                         , sxc.TAG_FIRSTLAYER
                         , sxc.ATTR_ITEM_ID_INT
                         , sxc.TAG_NAME);
}

bool StartupXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , sxc.TAG_ROOT
                                     , sxc.TAG_FIRSTLAYER
                                     , sxc.ATTR_ITEM_ID_INT
                                     , itemlist);
}

void StartupXmlBuilder::createDocument()
{
    createXmlBaseDocument(sxc.TAG_ROOT);
}

int StartupXmlBuilder::count()
{
    return getElementItemsCount(sxc.TAG_FIRSTLAYER);
}

void StartupXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == sxc.TAG_NAME
            || element == sxc.TAG_PROFILE
            || element == sxc.TAG_SCHEDULED
            || element == sxc.TAG_SCHEDULETYPE
            || element == sxc.TAG_DATETIME
            || element == sxc.TAG_SECOND
            || element == sxc.TAG_TIME
            || element == sxc.TAG_DATE
            || element == sxc.TAG_UNIQUE)
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
