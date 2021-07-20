/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef FILESEARCHLOADER_H
#define FILESEARCHLOADER_H

#include "filesearchloader_global.h"
#include <QDateTime>
#include "basefilesearch.h"
#include <../searchxmlbuilder/searchxmlbuilder.h>
#include <../share/variantconverter.h>


class FILESEARCHLOADERSHARED_EXPORT FileSearchLoader : public BaseFileSearch
{
public:
    explicit FileSearchLoader(QObject *parent = nullptr);
    virtual ~FileSearchLoader() override;

    QStringList searchFromXml(int itemid);
    QStringList searchFromStrList(QList<QStringList> *list);
private:
    SearchXmlBuilder *builder;
    long long stringToLongLong(QString num);
};

#endif // FILESEARCHLOADER_H
