/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
    inline void clearFileText(){ file->resize(0); }
    bool openFile(QFlags<QIODevice::OpenModeFlag> flags);

    // close file if it is opened
    inline void closeFile(){ if(file->isOpen()) file->close(); }

    void checkXmlError();
    QString getTabbedXmlString(QTemporaryFile *tmp, int indent);
    QString appendTabIndent(int num);

    // LF (Unix style)
    inline QString endLineStr(){ return "\n"; }

    QXmlStreamReader *rxml;
    QXmlStreamWriter *wxml;
    QFile *file;
};

#endif // BASEXMLBUILDER_H
