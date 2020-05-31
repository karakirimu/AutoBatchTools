/*
 * Copyright 2016-2019 karakirimu
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

#ifndef BASEXMLBUILDER_H
#define BASEXMLBUILDER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QObject>
#include <QRegularExpression>
#include <QTemporaryFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QTextCodec>
#include "xmlbuilder_global.h"

class XMLBUILDERSHARED_EXPORT BaseXmlBuilder : public QObject
{
    Q_OBJECT
public:
    explicit BaseXmlBuilder(QObject *parent = nullptr);
    virtual ~BaseXmlBuilder();

//    void createXmlBaseDocument(QString rootelement);
    void createXmlBaseDocument(QString rootelement, QString rootattr = "", QString rootattrvalue = "");
    int getElementItemsCount(QString element);

protected:

    void setFileName(QString filename);
    bool deleteElementGroup(QString element, QString attr, int value, bool withparent);
    qint64 getElementFirstLineNumber(QString element);
    qint64 getElementFirstLineNumber(QString element, QString attr, QString attrvalue);
    qint64 getElementEndLineNumber(QString element, QString attr, int value);
    void openedFileReset();
    void clearFileText();
    bool openFile(QFlags<QIODevice::OpenModeFlag> flags);

    //close file if it is opened
    void closeFile();

    void checkXmlError();
    QString getTabbedXmlString(QTemporaryFile *tmp, int indent);
    QString appendTabIndent(int num);

    //All unified with LF (Unix style)
    QString endLineStr();

    QXmlStreamReader *rxml;
    QXmlStreamWriter *wxml;
    QFile *file;
};

#endif // BASEXMLBUILDER_H
