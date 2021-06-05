/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STRINGXMLBUILDER_H
#define STRINGXMLBUILDER_H

#include "stringxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class StringSettings_xml
 * @brief QList<QStringList> short description
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
    ~StringXmlBuilder() override;

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool insertItem(int itemid, QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);

    int count();
    void createVarElement(QList<QStringList> *internal, QStringList *var);

private:
    void setSearchItemData(QString element, QList<QStringList> *list) override;
    void createDocument();

    const QString ROOTELEMENT = "string";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";

    const QString STRING_VARIANT = "variant";
    const QString STRING_VALUE = "value";
};

#endif // STRINGXMLBUILDER_H
