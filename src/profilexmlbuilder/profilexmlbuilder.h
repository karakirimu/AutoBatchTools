/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILEXMLBUILDER_H
#define PROFILEXMLBUILDER_H

#include "profilexmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>


/**
 * @class The ProfileXmlBuilder class
 * @brief QList<QStringList> short description
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :name     :text   : profile name
 * 1    :desc     :text   : processxmlbuilder's description
 * 2    :file     :text   : profile file path
 */
class PROFILEXMLBUILDERSHARED_EXPORT ProfileXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit ProfileXmlBuilder(QObject *parent = nullptr);
    ~ProfileXmlBuilder();

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();

    enum {NAME, DESCRIPTON, FILE};
private:

    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "profilelist";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // PROFILEXMLBUILDER_H
