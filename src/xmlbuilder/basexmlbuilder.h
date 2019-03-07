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

    void createXmlBaseDocument(QString rootelement);
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
