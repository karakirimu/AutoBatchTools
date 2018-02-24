#include "basexmlbuilder.h"

BaseXmlBuilder::BaseXmlBuilder(QObject *parent) : QObject(parent)
{
    file = new QFile();
    rxml = new QXmlStreamReader();
    wxml = new QXmlStreamWriter();

    //set codec
    //locale settings
#ifdef Q_OS_WIN
    wxml->setCodec(QTextCodec::codecForName("Shift_JIS"));
#else
    wxml->setCodec(QTextCodec::codecForName("UTF-8"));
#endif
}

BaseXmlBuilder::~BaseXmlBuilder()
{
    if(file->isOpen()){
        file->close();
    }
    delete file;
    delete rxml;
    delete wxml;
}

void BaseXmlBuilder::setFileName(QString filename){
    file->setFileName(filename);

    if(openFile(QIODevice::ReadOnly)){
        file->close();
//        qDebug() << "file opened.";
    }else{
        qDebug() << "BaseXmlBuilder::make newfile";
        //create new file
        QFile tmpfilemake(filename);
        if(tmpfilemake.open(QIODevice::WriteOnly)){
            tmpfilemake.close();
            setFileName(filename);
        }
    }
}

void BaseXmlBuilder::createXmlBaseDocument(QString rootelement)
{
    openFile(QIODevice::WriteOnly);
    //delete older characters
    clearFileText();

    //set device
    wxml->setDevice(file);

    //seek specified line
    wxml->setAutoFormatting(true);

    //set 1 tabs
    wxml->setAutoFormattingIndent(-1);
    wxml->writeStartDocument();
#ifdef Q_OS_WIN
    wxml->writeTextElement(rootelement, "\n");
#else
    wxml->writeTextElement(rootelement, "\r\n");
#endif
    wxml->writeEndDocument();

    closeFile();
}

//if it can't get specified element, this function does nothing.
bool BaseXmlBuilder::deleteSpecifiedElementGroup(QString element, QString attr, int value, bool withparent)
{
    bool hasid = false;
    int firstline = 0;
    int endline = 0;
    QString deletedText;

    openFile(QIODevice::ReadWrite);
    readFileReset();

    //get start and end line counter
    while (!rxml->atEnd() && !rxml->hasError())
    {
        rxml->readNext();
        if (rxml->isStartElement() && rxml->name().toString() == element)
        {
            QString val = rxml->attributes().value(attr).toString();

            if(val == QString::number(value)){
                hasid = true;
                firstline = rxml->lineNumber();
            }
        }

        if(rxml->isEndElement() && hasid && rxml->name() == element){
            hasid = false;
            endline = rxml->lineNumber();
            break;
        }

    }

    //xml error check
//    qDebug() << "BaseXmlBuilder: " << this->sender();
    checkXmlError();

    //reset file
    //? please load file readwritemode
    file->reset();
    QTextStream del(file);

    int linecount = 0;
    QString tmp;
    //read forword data set
    while(!del.atEnd()){
        tmp = del.readLine();
        linecount++;
        //外のタグは残らない
        if(withparent && (linecount < firstline || linecount > endline)){
            deletedText.append(tmp);
            //WARNING:hard coding
#ifdef Q_OS_WIN
            deletedText.append("\n");
#else
            deletedText.append("\r\n");
#endif
        }
        //以上以下にすると外のタグだけは残る
        if(!withparent && (linecount <= firstline || linecount >= endline)){
            deletedText.append(tmp);
            //WARNING:hard coding
#ifdef Q_OS_WIN
            deletedText.append("\n");
#else
            deletedText.append("\r\n");
#endif
        }
    }

    clearFileText();
//    qDebug() << "firstline:" << firstline;
//    qDebug() << "endline:" << endline;
//    qDebug() << deletedText;

    //add text
    del << deletedText;

    closeFile();
    return true;
}

// get first line in multiple specified element.
int BaseXmlBuilder::getSpecifiedElementLineFirst(QString element)
{
    int line = 0;

    openFile(QIODevice::ReadOnly);
    readFileReset();

    while (!rxml->atEnd() && !rxml->hasError())
    {
        rxml->readNext();
        if (rxml->isStartElement() && rxml->name().toString() == element)
        {
            line = rxml->lineNumber();
            break;
        }
    }

//    qDebug() << "BaseXmlBuilder: " << element;
    checkXmlError();
//    qDebug() << line << ":linefirst";
    closeFile();
    return line;
}

// it is not have any elements, then returns -1.
int BaseXmlBuilder::getSpecifiedElementLineFirst(QString element, QString attr, QString attrvalue)
{
    int line = -1;

    openFile(QIODevice::ReadOnly);
    readFileReset();

    while (!rxml->atEnd() && !rxml->hasError())
    {
        rxml->readNext();
        if (rxml->isStartElement()
                && rxml->name().toString() == element
                && rxml->attributes().value(attr) == attrvalue)
        {
            line = rxml->lineNumber();
            break;
        }
    }

//    qDebug() << "BaseXmlBuilder: " << element << attr << attrvalue;
    checkXmlError();
    //qDebug() << line << ":line";
    closeFile();
    return line;
}

int BaseXmlBuilder::getSpecifiedElementLineEnd(QString element, QString attr, int value)
{
    int line = 0;
    bool flags = false;

    openFile(QIODevice::ReadOnly);
    readFileReset();

    while (!rxml->atEnd() && !rxml->hasError())
    {
        rxml->readNext();
        if (rxml->isStartElement() && rxml->name().toString() == element)
        {
            QString val = rxml->attributes().value(attr).toString();
            if(value == val.toInt()) flags = true;
        }

        if(flags && rxml->isEndElement() && rxml->name().toString() == element){
            flags = false;
            line = rxml->lineNumber();
            break;
        }
    }

//    qDebug() << "BaseXmlBuilder: " << line << attr << value;
    checkXmlError();
    closeFile();
    return line;
}

int BaseXmlBuilder::getSpecifiedElementItemsCount(QString element)
{
    int count = 0;
    //reset
    openFile(QIODevice::ReadOnly);
    readFileReset();

    while (!rxml->atEnd() && !rxml->hasError())
    {
        rxml->readNext();
        if (rxml->isStartElement() && rxml->name().toString() == element)
        {
            count++;
        }
    }

//    qDebug() << "BaseXmlBuilder: " << element;
    checkXmlError();
    closeFile();
//    qDebug() << count << " :count";
    return count;
}

// return numbers of tab sequence
// Windows:
QString BaseXmlBuilder::appendTabIndent(int num)
{
    QString indent;
    for(int i = 0; i< num; i++){
        indent.append("\t");
    }
    return indent;
}

void BaseXmlBuilder::checkXmlError()
{
    if (rxml->hasError())
    {
        qDebug() << "rxml error: " << rxml->errorString();
    }
}

void BaseXmlBuilder::readFileReset()
{
    //rxml devicereset ?
//    if(rxml->atEnd()){
//        qDebug() << "Reached end, done";
//        rxml->device()->reset();
//    }

    //file seek to first point
    file->reset();

    //set new device
    rxml->setDevice(file);
}

void BaseXmlBuilder::clearFileText()
{
    file->resize(0);
}

bool BaseXmlBuilder::openFile(QFlags<QIODevice::OpenModeFlag> flags)
{
    //fileopen check
    if (!file->open(flags | QFile::Text)) {
        qDebug() << "Cannot read file.";
        return false;
    }

    return true;
}

void BaseXmlBuilder::closeFile()
{
    //check file is opened
    if(file->isOpen()){
        file->close();
    }
}


/**
 * @brief BaseXmlBuilder::getAdjustedXmlDataString
 * @param tmp
 * @param indent
 * @return
 * @details adjust code formatting. it becomes able to insert created xml.
 */
QString BaseXmlBuilder::getAdjustedXmlDataString(QTemporaryFile *tmp, int indent)
{
    if(tmp->open()){
        QTextStream in(tmp);

        QString appenddata = in.readAll();
        QRegularExpression re;

        //Windows
#ifdef Q_OS_WIN
        re.setPattern("\n");
        appenddata.replace( re, "\n" + appendTabIndent(indent));
#else
        //other
        re.setPattern("\r\n");
        appenddata.replace( re, "\r\n" + appendTabIndent(indent));
#endif
        //qDebug() << appenddata;

        tmp->close();

        //delete first \n code
        appenddata.remove(0,1);

        //append to last character
#ifdef Q_OS_WIN
        appenddata.append("\n");
#else
        appenddata.append("\r\n");
#endif
        return appenddata;
    }

    return nullptr;
}
