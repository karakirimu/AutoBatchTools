/*
 * Copyright 2016-2020 karakirimu
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

#ifndef PROCESSXMLLISTGENERATOR_H
#define PROCESSXMLLISTGENERATOR_H

#include "processxmllistgenerator_global.h"
#include <QObject>
#include <QHash>
#include <QDateTime>

#include <QDebug>

/**
 * @brief The XmlListGenerator class
 * This class can create default list (to xml) and operate xml states.
 */
class PROCESSXMLLISTGENERATORSHARED_EXPORT ProcessXmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ProcessXmlListGenerator(QObject *parent = nullptr);
    ~ProcessXmlListGenerator();

    //tab and list connection map
    enum{NORMAL, SEARCH, PLUGINS, OTHER};

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    //get structure firstpos info
    void getListStructure(QList<QStringList> *ctos, QHash<int, int> *posinfo);

    //check info
    int getType(QString type);
    int fetchCmdFirstPos(QString tag, const QList<QStringList> *loadbase);

    //take specific item string
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);
    QString fetch(QString tag, QString value, QString attr,const QList<QStringList> *loadbase);

private:

    //xml attributes (temp)

    // exec(normal)
    const QString E_CMD                      = "exc";
    const QString E_CMDARGCOUNT              = "cmdc";

    // plugin(extrafunc)
    const QString PL_CMD                     = "plc";
    const QString PL_CMDARGCOUNT             = "pcmdc";

    // value define (index 1)
    const QString TYPE_EXEC                  = "normal";
    const QString TYPE_SEARCH                = "search";
    const QString TYPE_SCRIPT                = "script";
    const QString TYPE_ANOTHER               = "other";

    // attribute define (index 2)
    // all
    const QString ATTR_NONE                  = "";

};

#endif // PROCESSXMLLISTGENERATOR_H
