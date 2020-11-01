/*
 * Copyright 2016-2020 karakirimu
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

bool StartupXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
    return Xmlbuilder::readItem(itemid, FIRSTLAYER, ATTR, itemlist);
}

bool StartupXmlBuilder::addItem(const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(count(), ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

bool StartupXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, FIRSTLAYER, ATTR);
}

bool StartupXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StartupXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, ROOTELEMENT, FIRSTLAYER, ATTR);
}

void StartupXmlBuilder::copyItem(int itemid)
{
    Xmlbuilder::copyItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, "name");
}

bool StartupXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, ROOTELEMENT, FIRSTLAYER, ATTR, itemlist);
}

void StartupXmlBuilder::createDocument()
{
    createXmlBaseDocument(ROOTELEMENT);
}

int StartupXmlBuilder::count()
{
    return getElementItemsCount(FIRSTLAYER);
}

void StartupXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
    if(element == "name"
            || element == "prof"
            || element == "valid"
            || element == "radio"
            || element == "schdt"
            || element == "schsec"
            || element == "scht"
            || element == "schday"
            || element == "unique")
    {
        QStringList data;
        //add element and text
        data.append(element);
        data.append(rxml->readElementText());
        list->append(data);
    }
}
