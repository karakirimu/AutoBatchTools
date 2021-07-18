/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SEARCHXMLBUILDER_H
#define SEARCHXMLBUILDER_H

#include "searchxmlbuilder_global.h"
#include <QObject>
#include <../xmlbuilder/xmlbuilder.h>

constexpr char SEARCH_NAME[]      = "name";
constexpr char SEARCH_KEYWORD[]   = "keyword";
constexpr char SEARCH_REGEX[]     = "regex";
constexpr char SEARCH_DIR[]       = "dir";
constexpr char SEARCH_RECURSIVE[] = "recursive";
constexpr char SEARCH_SECONDS[]   = "seconds";
constexpr char SEARCH_CREATION[]  = "creation";
constexpr char SEARCH_MODIFIED[]  = "modified";
constexpr char SEARCH_FSIZE_1[]   = "fsize_1";
constexpr char SEARCH_FSIZE_2[]   = "fsize_2";

constexpr char ENABLED[]          = "enabled";
constexpr char COMBO[]            = "combo";
constexpr char SEARCH_NONE[]      = "";

/**
 * @class SearchXmlBuilder
 * @brief QList<QStringList> short description
 * @details this function sets text and checkbox.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:      2:      3:      4:     5:
 * 0    :name     :text   :
 * 1    :keyword  :text   :
 * 2    :regex    :text   :enabled:data   :
 * 3    :dir      :text   :
 * 4    :recursive:text   :
 * 5    :seconds  :text   :enabled:data   :
 * 6    :creation :text   :enabled:data   :combo   :data
 * 7    :modified :text   :enabled:data   :combo   :data
 * 8    :fsize_1  :text   :enabled:data   :combo   :data
 * 9    :fsize_2  :text   :enabled:data   :combo   :data
 */
class SEARCHXMLBUILDERSHARED_EXPORT SearchXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
   explicit SearchXmlBuilder(QObject *parent = nullptr);
   ~SearchXmlBuilder() override;

   bool readAll(QList<QList<QStringList> *> *itemlist);
   bool writeAll(const QList<QList<QStringList> *> *itemlist);

   bool readItem(int itemid, QList<QStringList> *itemlist);
   bool addItem(const QList<QStringList> *itemlist);
   bool deleteItem(int itemid);
   bool editItem(int itemid, const QList<QStringList> *itemlist);
   void swapItem(int beforeitemid, int afteritemid);
   void copyItem(int itemid);

   int count();

   QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);

private:
    void setSearchItemData(QString element, QList<QStringList> *list) override;
    void createDocument();

    const QString ROOTELEMENT = "search";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
    const QString ATTRVERSION = "version";
    const QString VERSION = "1.00";
};

#endif // SEARCHXMLBUILDER_H
