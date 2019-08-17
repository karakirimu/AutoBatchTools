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

#ifndef STRINGXMLBUILDER_H
#define STRINGXMLBUILDER_H

#include "stringxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class StringSettings_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :variant  :text   :
 * 1    :value    :text   :
 */
class STRINGXMLBUILDERSHARED_EXPORT StringXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit StringXmlBuilder(QObject *parent = nullptr);
    ~StringXmlBuilder();

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool insertItem(int itemid, QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();

    int count();
    void createVarElement(QList<QStringList> *internal, QStringList *var);

private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "string";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // STRINGXMLBUILDER_H
