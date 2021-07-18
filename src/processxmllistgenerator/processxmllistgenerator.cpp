/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "processxmllistgenerator.h"

ProcessXmlListGenerator::ProcessXmlListGenerator()
{

}

///DEPENDS_XML
void ProcessXmlListGenerator::createSeparateList(QList<QStringList> *ctos)
{
    //get selected index
    int index = fetch(ctos, pxc.TAG_FUNCTIONSELECT).toInt();

    //remove till first header
    while(!ctos->empty()){
        if(getType(ctos->first().at(1)) == index){
            break;
        }else{
            ctos->removeFirst();
        }
    }

    int i = 1;
    bool rem = false;
    while(i < ctos->count()){
        if(getType(ctos->at(i).at(1)) == -1 && !rem){
            i++;
        }else{
            rem = true;
            ctos->removeAt(i);
        }
    }
}

/**
 * @fn ProcessXmlListGenerator::getType
 * @brief Returns the GUI correspondence corresponding to the XML tag.
 * @param type XML tag.
 * @return The type of the corresponding tab UI.
 */
int ProcessXmlListGenerator::getType(QString type)
{
    if(type == fs.getString(fs.TYPE::EXECUTE))     return static_cast<int>(TAB::EXECUTE);
    if(type == fs.getString(fs.TYPE::FILESEARCH))  return static_cast<int>(TAB::FILESEARCH);
    if(type == fs.getString(fs.TYPE::PLUGIN))      return static_cast<int>(TAB::PLUGINS);
    if(type == fs.getString(fs.TYPE::PROFILELOAD)) return static_cast<int>(TAB::PROFILELOAD);
    return static_cast<int>(TAB::INVALID);
}

/**
 * @fn ProcessXmlListGenerator::fetchCommandFirstPos
 * @brief Find the first position in the list that contains
 *        nformation about the table elements in the tab.
 * @param tag Tag name. (TAG_E_CMD_HA1 or TAG_P_CMD_HA1)
 * @param loadbase List of elements selected in ProcessFlowTable.
 * @return The first position in the list of table elements, or -1 if not found.
 */
int ProcessXmlListGenerator::fetchCommandFirstPos(QString tag
                                              , const QList<QStringList> *loadbase)
{
    qsizetype count = loadbase->count();
    int i = 0;

    if(tag == pxc.TAG_E_CMD_HA1)  tag = pxc.TAG_E_COMMANDCOUNT_INT;
    if(tag == pxc.TAG_P_CMD_HA1) tag = pxc.TAG_P_COMMANDCOUNT_INT;

    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            return i + 1;
        }
        i++;
    }

    //cannot find
    return -1;
}

QString ProcessXmlListGenerator::fetch(QString tag
                                       , const QList<QStringList> *loadbase)
{
    return this->fetch(loadbase, tag);
}

// If there is no element of "attr", assign PROCESS_NONE to "attr"
QString ProcessXmlListGenerator::fetch(QString tag
                                       , QString attr
                                       , const QList<QStringList> *loadbase)
{
    return this->fetch(loadbase, tag, attr);
}

QString ProcessXmlListGenerator::fetch(QString tag
                                       , QString attr
                                       , const QList<QStringList> *loadbase
                                       , int firstpos)
{
    return this->fetch(loadbase, tag, attr, "", firstpos);
}

QString ProcessXmlListGenerator::fetch(QString tag
                                       , QString value
                                       , QString attr
                                       , const QList<QStringList> *loadbase)
{
    return this->fetch(loadbase, tag, attr, value);
}

const QString ProcessXmlListGenerator::fetch(const QList<QStringList> *loadbase
                                             , QString tag
                                             , QString attr
                                             , int firstpos)
{
    return this->fetch(loadbase, tag, attr, "", firstpos);
}

//const QString ProcessXmlListGenerator::fetch(const QList<QStringList> *loadbase
//                                             , QString tag
//                                             , QString value
//                                             , QString attr)
//{
//    return this->fetch(loadbase, tag, attr, value);
//}

/**
 * @fn ProcessXmlListGenerator::fetch
 * @brief
 * This function gets the value from the selected XML tag, tag item, and attribute.
 * It can be used to get a specific value from the read result of a class
 * that inherits from XmlBuilder.
 *
 * @param loadelements Elements retrieved from XmlBuilder. (Required)
 * @param tag          The name of the tag to retrieve. (Required)
 * @param attr         The attribute to retrieve. (Optional)
 * @param tagvalue     The value of the tag to retrieve. (Optional)
 * @param firstpos     The first row number to search in loadelements. (Optional)
 * @return Selected item or empty.
 */
const QString ProcessXmlListGenerator::fetch(const QList<QStringList> *loadelements
                                       , QString tag
                                       , QString attr
                                       , QString tagvalue
                                       , int firstpos)
{
    qsizetype count = loadelements->count();
    qsizetype listnummax = 0;

    auto valueCondition = [&](QString value, QString elementvalue){
        return (value == EMPTY || value == elementvalue);
    };

    int i = firstpos;
    while(i < count){
        if(tag == loadelements->at(i).first()
            && valueCondition(tagvalue, loadelements->at(i).at(1))){

            if(attr == EMPTY){
                return loadelements->at(i).at(1);
            }

            listnummax = loadelements->at(i).count();

            for(int j = 3; j < listnummax; j += 2){
                if(attr == loadelements->at(i).at(j-1)){
                    return loadelements->at(i).at(j);
                }
            }
        }
        i++;
    }

    //cannot find
    return EMPTY;
}
