/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROFILEXMLBUILDER_H
#define PROFILEXMLBUILDER_H

#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class The ProfileXmlBuilder class
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :name     :text   :
 * 1    :desc     :text   :
 * 2    :file     :text   :
 */
class ProfileXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit ProfileXmlBuilder(QObject *parent = nullptr);
    virtual ~ProfileXmlBuilder() override;

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();
private:

    void setSearchItemData(QString element, QList<QStringList> *list) override;
    const QString ROOTELEMENT = "profilelist";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // PROFILEXMLBUILDER_H
