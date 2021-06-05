/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PLUGINSXMLBUILDER_H
#define PLUGINSXMLBUILDER_H

#include "pluginsxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>
#include <QDir>

/**
 * @class PLUGINSXmlBuilder
 * @brief QList<QStringList> short description
 * @details
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :manual   :text   :
 */

class PLUGINSXMLBUILDERSHARED_EXPORT PluginsXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit PluginsXmlBuilder(QObject *parent = nullptr);
    ~PluginsXmlBuilder() override;

    bool readItem(int itemid, QList<QStringList> *itemlist);
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    void copyItem(int itemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();

    const QString PL_XML_NAME = "manual";
    const QString PL_XML_ATTR_FILE = "file";

private:
    void setSearchItemData(QString element, QList<QStringList> *list) override;

    const QString ROOTELEMENT = "plugins";
    const QString FIRSTLAYER = "item";
    const QString ATTR = "id";
};

#endif // PLUGINSXMLBUILDER_H
