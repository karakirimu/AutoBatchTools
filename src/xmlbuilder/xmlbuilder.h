/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include "xmlbuilder_global.h"
#include "basexmlbuilder.h"
#include <QObject>

/**
 * @class Xmlbuilder
 * @brief This class was created for easy editing of two-level XML files.
 *        By inheriting this class, we can edit various profiles of XML.
 *
 * @details This class edits a two-level XML file. To be precise, an XML file
 *          with the following structure can be generated and edited.
 *          The tag name matches the function argument name.
 *          The layer enclosed by firstlayer is retrieved to QList<QStringList>
 *          using setSearchItemData function.
 *
 *          <rooteelement>
 *            <firstlayer attr="itemid(integer value)">
 *           	<element attr="">value</element>
 *          	<element></element>
 *              .
 *              .
 *              (Data hierarchy that can be read from the setSearchItemData function.)
 *              .
 *              .
 *            </firstlayer>
 *          </rooteelement>
 *
 * @remarks In the returned QList <QStringList>, the layers below the firstlayer are
 *          stored in the QStringList line by line. QStringList is generally stored
 *          in the following order.
 *
 *          Tag name, value, attribute name, attribute value, attribute name, ...
 *
 *          Refer to the inherited class of XmlBuilder for implementation.
 */
class XMLBUILDERSHARED_EXPORT Xmlbuilder : public BaseXmlBuilder
{
    Q_OBJECT
protected:
    explicit Xmlbuilder(QObject *parent = nullptr);
    virtual ~Xmlbuilder() override;

    bool readItem(int itemid
                  , QString firstlayername
                  , QString attr
                  , QList<QStringList> *itemlist);

    bool readAllItem(QString firstlayername
                     , QString attr
                     , QList<QList<QStringList> *> *itemlist);

    bool writeAllItem(QString root
                      , QString rootattr
                      , QString rootattrvalue,
                      QString firstlayername
                      , QString attr,
                      const QList<QList<QStringList> *> *itemlist);

    bool deleteItem(int itemid
                    , QString firstlayername
                    , QString attr);

    void swapItem(int beforeitemid
                  , int afteritemid
                  , QString rootelement
                  , QString firstlayername
                  , QString attr);

    bool overwriteItem(int itemid
                       , QString rootelement
                       , QString firstlayername
                       , QString attr
                       , const QList<QStringList> *itemlist);

    bool insertItem(int itemid
                    , QString rootelement
                    , QString firstlayername
                    , QString attr
                    , const QList<QStringList> *itemlist);

    void copyItem(int itemid
                  , QString rootelement
                  , QString firstlayername
                  , QString attr
                  , QString exchangedata);

    /**
     * @fn Xmlbuilder::setSearchItemData
     * @brief Implements a method of assigning a value to QStringList in QList
     *        used when retrieving the specified element.
     *
     * @param element : An element that requires an assignment method.
     * @param list    : A list where items are set. (same as itemlist)
     */
    virtual void setSearchItemData(QString element
                                   , QList<QStringList> *list) = 0;

private:
    QString getStructureStr(int itemid
                            , QString firstlayername
                            , QString attr
                            , int indentnum
                            , const QList<QStringList> *list
                            , bool withparent);

    qint64 getItemFirstLine(int tablenum, QString firstlayername, QString attr);

    void writeXmlItem(int itemid
                      , QString firstlayername
                      , QString attr
                      , const QList<QStringList> *list);

    void writeElementData(const QList<QStringList> *list);

    bool insertItemId(int itemid
                      , QString rootelement
                      , QString firstlayername
                      , QString attr
                      , const QList<QStringList> *itemlist);

    bool deleteItemId(int destitemid
                      , int fromitemid
                      , QString firstlayername
                      , QString attr);

    bool deleteItemIdToEnd(int deleteitemid, QString firstlayername, QString attr);

    int count(QString firstlayername);
};

#endif // XMLBUILDER_H
