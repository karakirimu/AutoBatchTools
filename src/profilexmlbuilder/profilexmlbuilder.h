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

constexpr char PROFILE_NAME[]        = "name";
constexpr char PROFILE_DESCRIPTION[] = "desc";
constexpr char PROFILE_FILE[]        = "file";

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
    virtual ~ProfileXmlBuilder() override;

    bool readAll(QList<QList<QStringList> *> *itemlist);
    bool writeAll(const QList<QList<QStringList> *> *itemlist);

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

    const QString ROOTELEMENT = "profilelist";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
    const QString ATTRVERSION = "version";
    const QString VERSION = "1.00";
};

#endif // PROFILEXMLBUILDER_H
