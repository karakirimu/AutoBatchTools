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
class PROCESSXMLLISTGENERATORSHARED_EXPORT ProcessXmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ProcessXmlListGenerator(QObject *parent = nullptr);
    ~ProcessXmlListGenerator();

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    int fetchCmdFirstPos(QString tag, const QList<QStringList> *loadbase);

    //take specific item string
    QString fetch(QString tag, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);
    QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase, int firstpos);
    QString fetch(QString tag, QString value, QString attr,const QList<QStringList> *loadbase);

private:
    int getType(QString type);

    ProcessXmlConstant pxc;
    FunctionType fs;

    // attribute define (index 2)
    // all
    const QString ATTR_NONE                  = "";

};

#endif // PROCESSXMLLISTGENERATOR_H
