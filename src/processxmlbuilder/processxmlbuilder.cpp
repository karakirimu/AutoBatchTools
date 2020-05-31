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

#include "processxmlbuilder.h"

ProcessXmlBuilder::ProcessXmlBuilder(QObject *)
{

}

ProcessXmlBuilder::~ProcessXmlBuilder()
{

}

void ProcessXmlBuilder::setLoadPath(QString filepath)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::setLoadPath] : " << filepath;
#endif

    if(filepath != ""){
        //set new file
        setFileName(filepath);

        if(count() == 0) createDocument();

    }else{
        //re create file object
        if(file->isOpen()) file->close();

        delete file;
        file = new QFile();

    }
}

void ProcessXmlBuilder::setLoadBlankPath(QString filepath)
{
    if(filepath != ""){
        //set new file
        setFileName(filepath);

        if(count() == 0){
            createXmlBaseDocument(pxc.TAG_ROOT, pxc.ATTR_ROOT, pxc.ATTRVALUE_ROOT);
        }

    }
}

//void ProcessXmlBuilder::delLoadPath()
//{
//    if(file->exists()){
//        file->close();
//        if(!file->remove()){
//            qDebug() << file->errorString();
//        }
//    }
//}

bool ProcessXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::readItem] :" << itemid;
#endif
    return Xmlbuilder::readItem(itemid, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT, itemlist);
}

bool ProcessXmlBuilder::readAllItem(QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::readAllItem]";
#endif
    return Xmlbuilder::readAllItem(pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT, itemlist);
}

bool ProcessXmlBuilder::writeAllItem(const QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::writeAllItem]";
#endif
    return Xmlbuilder::writeAllItem(pxc.TAG_ROOT, pxc.ATTR_ROOT, pxc.ATTRVALUE_ROOT,
                                    pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT,
                                    itemlist);
}

bool ProcessXmlBuilder::insertItem(const QList<QStringList> *itemlist, int index)
{
    return Xmlbuilder::overwriteItem(index, pxc.TAG_ROOT, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT, itemlist);
}

bool ProcessXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT);
}

bool ProcessXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, pxc.TAG_ROOT, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT, itemlist);
}

void ProcessXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid, afteritemid, pxc.TAG_ROOT, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT);
}

bool ProcessXmlBuilder::overwriteItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid, pxc.TAG_ROOT, pxc.TAG_FIRSTLAYER, pxc.ATTR_COMMAND_ID_INT, itemlist);
}

void ProcessXmlBuilder::createDocument()
{
    createXmlBaseDocument(pxc.TAG_ROOT, pxc.ATTR_ROOT, pxc.ATTRVALUE_ROOT);
    QList<QStringList> newlist;

    //info element
    newlist.append((QStringList() << pxc.TAG_TYPE << fs.getString(fs.TYPE::INFORMATION)));
    newlist.append((QStringList() << pxc.TAG_I_NAME << ""));
    newlist.append((QStringList() << pxc.TAG_I_VERSION << ""));
    newlist.append((QStringList() << pxc.TAG_I_AUTHOR << ""));
    newlist.append((QStringList() << pxc.TAG_I_DESCRIPTION << ""));
//    newlist.append((QStringList() << FILEINPUT << "yes" << FILEINPUT_SEARCHCHECK << "no"));
    newlist.append((QStringList() << pxc.TAG_I_FILEINPUT_BOOL << "yes"));
    newlist.append((QStringList() << pxc.TAG_I_FILEINPUT_SEARCH_BOOL << "no"));
    newlist.append((QStringList() << pxc.TAG_I_FILESEARCH_NAME << "" << pxc.ATTR_COMMAND_ID_INT << ""));
    newlist.append((QStringList() << pxc.TAG_I_PROCESS_BOOL_HA1 << "yes" << pxc.ATTR_I_PROCESSMAX_INT << "1"));
    newlist.append((QStringList() << pxc.TAG_I_ARG_IN_ONELOOP_INT << "1"));
    newlist.append((QStringList() << pxc.TAG_I_RECURSIVE_LOOPMAX_INT << "0"));

    insertItem(&newlist, count());
    newlist.clear();

    //local element
    newlist.append((QStringList() << pxc.TAG_TYPE << fs.getString(fs.TYPE::LOCAL)));
    newlist.append((QStringList() << pxc.TAG_L_VARIANTCOUNT_INT << "0"));

    insertItem(&newlist, count());
    newlist.clear();
}

/**
 * @fn ProcessXmlBuilder::count
 * @brief Get the number of a specific tag group from XML.
 * @attention When run continuously, it's pretty heavy.
 * @return Specified tag group counts.
 */
int ProcessXmlBuilder::count()
{
    return getElementItemsCount(pxc.TAG_FIRSTLAYER);
}

void ProcessXmlBuilder::setSearchItemData(QString element, QList<QStringList> *list)
{
//    if(element.contains(QRegularExpression("^(name|ver|var|author|desc|finput|sinput|rlarg|reloop|cmdc|localc|sep|file|istack)$")))
//    {
//        //add element and text
//        list->append(QStringList() << element << rxml->readElementText());
//    }

    if(element == pxc.TAG_I_NAME
            || element == pxc.TAG_I_VERSION
            || element == pxc.TAG_FS_VARIANT
            || element == pxc.TAG_I_AUTHOR
            || element == pxc.TAG_I_DESCRIPTION
            || element == pxc.TAG_I_FILEINPUT_BOOL
            || element == pxc.TAG_I_FILEINPUT_SEARCH_BOOL
            || element == pxc.TAG_I_ARG_IN_ONELOOP_INT
            || element == pxc.TAG_I_RECURSIVE_LOOPMAX_INT
            || element == pxc.TAG_E_COMMANDCOUNT_INT
            || element == pxc.TAG_E_DETACH_BOOL
            || element == pxc.TAG_L_VARIANTCOUNT_INT
            || element == pxc.TAG_FS_SEPARATOR
            || element == pxc.TAG_FS_WRITEOPTION_INT
            || element == pxc.TAG_PLOAD_FILEPATH
            || element == pxc.TAG_P_COMMANDCOUNT_INT
            || element == pxc.TAG_P_NAME
            || element == pxc.TAG_P_FILEPATH
            || element == pxc.TAG_FUNCTIONSELECT)
    {
        //add element and text
        list->append(QStringList() << element << rxml->readElementText());
    }

    if(element == pxc.TAG_TYPE)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_ONLY_SCHEDULER_BOOL << rxml->attributes().value(pxc.ATTR_ONLY_SCHEDULER_BOOL).toString());
    }

    if(element == pxc.TAG_E_TIMEOUT_BOOL_HA1)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_TIMEOUT_INT << rxml->attributes().value(pxc.ATTR_TIMEOUT_INT).toString());
    }

    if(element == pxc.TAG_I_PROCESS_BOOL_HA1)
    {
        //add element and text, attributes and data
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_I_PROCESSMAX_INT << rxml->attributes().value(pxc.ATTR_I_PROCESSMAX_INT).toString());
    }

    if(/*element == ALL_CMDVALUE*/
            element == pxc.TAG_E_CMD_HA1
            || element == pxc.TAG_P_CMD_HA1
            || element == pxc.TAG_FS_NAME_HA1
            || element == pxc.TAG_I_FILESEARCH_NAME)
    {
        //add to QList
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_COMMAND_ID_INT << rxml->attributes().value(pxc.ATTR_COMMAND_ID_INT).toString());
    }

    if(element == pxc.TAG_L_VARIANT_HA1)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_L_VALUE << rxml->attributes().value(pxc.ATTR_L_VALUE).toString());
    }

    if(element == pxc.TAG_FS_FILEPATH_HA1)
    {
        list->append(QStringList() << element << rxml->readElementText()
                     << pxc.ATTR_FS_OUTPUTOPTION_INT << rxml->attributes().value(pxc.ATTR_FS_OUTPUTOPTION_INT).toString());
    }

}
