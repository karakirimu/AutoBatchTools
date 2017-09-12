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
    XMLBUILDERSHARED_EXPORT explicit BaseXmlBuilder(QObject *parent = 0);
    XMLBUILDERSHARED_EXPORT virtual ~BaseXmlBuilder();

    XMLBUILDERSHARED_EXPORT void createXmlBaseDocument(QString rootelement);
    XMLBUILDERSHARED_EXPORT int getSpecifiedElementItemsCount(QString element);

protected:

    void setFileName(QString filename);
    bool deleteSpecifiedElementGroup(QString element, QString attr, int value, bool withparent);
    int getSpecifiedElementLineFirst(QString element);
    int getSpecifiedElementLineFirst(QString element, QString attr, QString attrvalue);
    int getSpecifiedElementLineEnd(QString element, QString attr, int value);
    void readFileReset();
    void clearFileText();
    bool openFile(QFlags<QIODevice::OpenModeFlag> flags);
    void closeFile();
    void checkXmlError();
    QString getAdjustedXmlDataString(QTemporaryFile *tmp, int indent);
    QString appendTabIndent(int num);

    QXmlStreamReader *rxml;
    QXmlStreamWriter *wxml;
    QFile *file;
};

#endif // BASEXMLBUILDER_H
