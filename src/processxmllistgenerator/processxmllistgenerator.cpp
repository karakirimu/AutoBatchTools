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

#include "processxmllistgenerator.h"

ProcessXmlListGenerator::ProcessXmlListGenerator(QObject *parent)
    : QObject (parent)
{

}

ProcessXmlListGenerator::~ProcessXmlListGenerator()
{

}

///DEPENDS_XML
void ProcessXmlListGenerator::createSeparateList(QList<QStringList> *ctos)
{
    //get selected index
    int index = static_cast<QString>(fetch(pxc.TAG_FUNCTIONSELECT, ctos)).toInt();

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
 * @fn ProcessXmlListGenerator::fetchCmdFirstPos
 * @brief Find the first position in the list that contains
 *        nformation about the table elements in the tab.
 * @param tag Tag name. (E_CMD or PL_CMD)
 * @param loadbase List of elements selected in ProcessFlowTable.
 * @return The first position in the list of table elements, or -1 if not found.
 */
int ProcessXmlListGenerator::fetchCmdFirstPos(QString tag, const QList<QStringList> *loadbase)
{
    int count = loadbase->count();
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

QString ProcessXmlListGenerator::fetch(QString tag, const QList<QStringList> *loadbase)
{
    return this->fetch(tag, ATTR_NONE, loadbase, 0);
}

// If there is no element of "attr", assign PROCESS_NONE to "attr"
QString ProcessXmlListGenerator::fetch(QString tag, QString attr, const QList<QStringList> *loadbase)
{
    return this->fetch(tag, attr, loadbase, 0);
}

QString ProcessXmlListGenerator::fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos)
{
    int count = loadbase->count();
    int i = firstpos;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)){
            if(attr == ATTR_NONE) return loadbase->at(i).at(1);

            listnummax = loadbase->at(i).count();
            if(listnummax > 3 && attr == loadbase->at(i).at(2)) return loadbase->at(i).at(3);
            if(listnummax > 5 && attr == loadbase->at(i).at(4)) return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return ATTR_NONE;
}

QString ProcessXmlListGenerator::fetch(QString tag, QString value, QString attr, const QList<QStringList> *loadbase)
{
    int count = loadbase->count();
    int i = 0;
    int listnummax = 0;
    while(i < count){
        if(tag == loadbase->at(i).at(0)
                && value == loadbase->at(i).at(1)){
            if(attr == ATTR_NONE) return loadbase->at(i).at(1);

            listnummax = loadbase->at(i).count();
            if(listnummax > 3 && attr == loadbase->at(i).at(2)) return loadbase->at(i).at(3);
            if(listnummax > 5 && attr == loadbase->at(i).at(4)) return loadbase->at(i).at(5);
        }
        i++;
    }

    //cannot find
    return ATTR_NONE;
}
