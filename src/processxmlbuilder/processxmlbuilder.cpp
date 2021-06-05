/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
            createXmlBaseDocument(pxc.TAG_ROOT
                                  , pxc.ATTR_ROOT
                                  , pxc.ATTRVALUE_ROOT);
        }
    }
}

bool ProcessXmlBuilder::readItem(int itemid, QList<QStringList> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::readItem] :" << itemid;
#endif
    return Xmlbuilder::readItem(itemid
                                , pxc.TAG_FIRSTLAYER
                                , pxc.ATTR_COMMAND_ID_INT
                                , itemlist);
}

bool ProcessXmlBuilder::readAllItem(QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::readAllItem]";
#endif
    return Xmlbuilder::readAllItem(pxc.TAG_FIRSTLAYER
                                   , pxc.ATTR_COMMAND_ID_INT
                                   , itemlist);
}

bool ProcessXmlBuilder::writeAllItem(const QList<QList<QStringList> *> *itemlist)
{
#ifdef QT_DEBUG
    qDebug() << "[ProcessXmlBuilder::writeAllItem]";
#endif
    return Xmlbuilder::writeAllItem(pxc.TAG_ROOT
                                    , pxc.ATTR_ROOT
                                    , pxc.ATTRVALUE_ROOT
                                    , pxc.TAG_FIRSTLAYER
                                    , pxc.ATTR_COMMAND_ID_INT
                                    , itemlist);
}

bool ProcessXmlBuilder::insertItem(const QList<QStringList> *itemlist, int index)
{
    return Xmlbuilder::overwriteItem(index
                                     , pxc.TAG_ROOT
                                     , pxc.TAG_FIRSTLAYER
                                     , pxc.ATTR_COMMAND_ID_INT
                                     , itemlist);
}

bool ProcessXmlBuilder::deleteItem(int itemid)
{
    return Xmlbuilder::deleteItem(itemid
                                  , pxc.TAG_FIRSTLAYER
                                  , pxc.ATTR_COMMAND_ID_INT);
}

bool ProcessXmlBuilder::editItem(int itemid, const QList<QStringList> *itemlist)
{
    return Xmlbuilder::overwriteItem(itemid
                                     , pxc.TAG_ROOT
                                     , pxc.TAG_FIRSTLAYER
                                     , pxc.ATTR_COMMAND_ID_INT
                                     , itemlist);
}

void ProcessXmlBuilder::swapItem(int beforeitemid, int afteritemid)
{
    Xmlbuilder::swapItem(beforeitemid
                         , afteritemid
                         , pxc.TAG_ROOT
                         , pxc.TAG_FIRSTLAYER
                         , pxc.ATTR_COMMAND_ID_INT);
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
    newlist.append((QStringList() << pxc.TAG_I_FILESEARCH_NAME << ""
                                  << pxc.ATTR_COMMAND_ID_INT << ""));
    newlist.append((QStringList() << pxc.TAG_I_PROCESS_BOOL_HA1 << "yes"
                                  << pxc.ATTR_I_PROCESSMAX_INT << "1"));
    newlist.append((QStringList() << pxc.TAG_I_ARG_IN_ONELOOP_INT << "1"));
    newlist.append((QStringList() << pxc.TAG_I_RECURSIVE_LOOPMAX_INT << "0"));
    newlist.append((QStringList() << pxc.TAG_I_PROFILE_BASEPATH << "./"));

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
    if(element == pxc.TAG_I_NAME
        || element == pxc.TAG_I_VERSION
        || element == pxc.TAG_FS_VARIANT
        || element == pxc.TAG_I_AUTHOR
        || element == pxc.TAG_I_DESCRIPTION
        || element == pxc.TAG_I_FILEINPUT_BOOL
        || element == pxc.TAG_I_FILEINPUT_SEARCH_BOOL
        || element == pxc.TAG_I_ARG_IN_ONELOOP_INT
        || element == pxc.TAG_I_PROFILE_BASEPATH
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
        || element == pxc.TAG_FUNCTIONSELECT
        || element == pxc.TAG_TYPE
        || element == pxc.TAG_E_TIMEOUT_BOOL_HA1
        || element == pxc.TAG_I_PROCESS_BOOL_HA1
        || element == pxc.TAG_E_CMD_HA1
        || element == pxc.TAG_P_CMD_HA1
        || element == pxc.TAG_FS_NAME_HA1
        || element == pxc.TAG_I_FILESEARCH_NAME
        || element == pxc.TAG_L_VARIANT_HA1
        || element == pxc.TAG_FS_FILEPATH_HA1) {

        QStringList read;
        QString attrname = "";

        for(auto &attr : rxml->attributes()){
            attrname = attr.name().toString();
            if(attrname == pxc.ATTR_ONLY_SCHEDULER_BOOL
                || attrname == pxc.ATTR_TIMEOUT_INT
                || attrname == pxc.ATTR_I_PROCESSMAX_INT
                || attrname == pxc.ATTR_COMMAND_ID_INT
                || attrname == pxc.ATTR_L_VALUE
                || attrname == pxc.ATTR_FS_OUTPUTOPTION_INT){
                read.append(attrname);
                read.append(attr.value().toString());
            }
        }

        read.prepend(rxml->readElementText());
        read.prepend(element);
        list->append(read);
    }
}
