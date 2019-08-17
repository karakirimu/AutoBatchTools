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

#ifndef SEARCHXMLBUILDER_H
#define SEARCHXMLBUILDER_H

#include "searchxmlbuilder_global.h"
#include <QObject>
#include <../xmlbuilder/xmlbuilder.h>

#define SEARCH_NAME         "name"
#define SEARCH_KEYWORD      "keyword"
#define SEARCH_REGEX        "regex"
#define SEARCH_DIR          "dir"
#define SEARCH_RECURSIVE    "recursive"
#define SEARCH_SECONDS      "seconds"
#define SEARCH_CREATION     "creation"
#define SEARCH_MODIFIED     "modified"
#define SEARCH_FSIZE_1      "fsize_1"
#define SEARCH_FSIZE_2      "fsize_2"

#define ENABLED     "enabled"
#define COMBO       "combo"
#define SEARCH_NONE ""

/**
 * @class SearchXmlBuilder
 * @brief QList<QStringList> short description
 * @param data
 * @details this function sets text and checkbox.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:      2:      3:      4:     5:
 * 0    :name     :text   :
 * 1    :keyword  :text   :
 * 2    :dir      :text   :
 * 3    :recursive:text   :
 * 4    :seconds  :text   :enabled:data   :
 * 5    :creation :text   :enabled:data   :combo   :data
 * 6    :modified :text   :enabled:data   :combo   :data
 * 7    :fsize_1  :text   :enabled:data   :combo   :data
 * 8    :fsize_2  :text   :enabled:data   :combo   :data
 */
class SEARCHXMLBUILDERSHARED_EXPORT SearchXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
   explicit SearchXmlBuilder(QObject *parent = nullptr);
   ~SearchXmlBuilder();

   bool readItem(int itemid, QList<QStringList> *itemlist);
   bool addItem(const QList<QStringList> *itemlist);
   bool deleteItem(int itemid);
   bool editItem(int itemid, const QList<QStringList> *itemlist);
   void swapItem(int beforeitemid, int afteritemid);
   void copyItem(int itemid);
   bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
   void createDocument();
   int count();
   QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "search";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // SEARCHXMLBUILDER_H
