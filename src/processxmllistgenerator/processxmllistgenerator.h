/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef PROCESSXMLLISTGENERATOR_H
#define PROCESSXMLLISTGENERATOR_H

#include "processxmllistgenerator_global.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

#include <QDebug>
#include <../share/process/processxmlconstant.h>
#include <../share/process/functiontype.h>

/**
 * @brief The XmlListGenerator class
 * This class can create default list (to xml) and operate xml states.
 */
class PROCESSXMLLISTGENERATORSHARED_EXPORT ProcessXmlListGenerator
{
public:
    ProcessXmlListGenerator();

    int fetchCommandFirstPos(QString tag, const QList<QStringList> *loadbase);

    // take specific item string
    const QString fetch(const QList<QStringList> *loadbase
                        , QString tag
                        , QString attr
                        , int firstpos);

    const QString fetch(const QList<QStringList> *loadelements
                  , QString tag
                  , QString attr = ""
                  , QString tagvalue = ""
                  , int firstpos = 0);

private:
    ProcessXmlConstant pxc;

    const QString EMPTY                  = "";
};

#endif // PROCESSXMLLISTGENERATOR_H
