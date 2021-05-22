/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "basexmlbuilder.h"

BaseXmlBuilder::BaseXmlBuilder(QObject *parent)
    : QObject(parent)
{
    file = new QFile();
    rxml = new QXmlStreamReader();
    wxml = new QXmlStreamWriter();
}

BaseXmlBuilder::~BaseXmlBuilder()
{
    closeFile();
    delete file;
    delete rxml;
    delete wxml;
}

/**
 * @fn BaseXmlBuilder::setFileName
 * @brief Specify the XML file name that this class opens.
 *        If the specified file does not exist, create it.
 *
 * @param filename : The path of the file to create or load.
 */
void BaseXmlBuilder::setFileName(QString filename){
    file->setFileName(filename);

    if(openFile(QIODevice::ReadOnly)){
        file->close();
//        qDebug() << "file opened.";
    }else{
        qDebug() << "[BaseXmlBuilder::setFileName] : make newfile";
        //create new file
        QFile tmpfilemake(filename);
        if(tmpfilemake.open(QIODevice::WriteOnly)){
            tmpfilemake.close();
            setFileName(filename);
        }
    }
}

/**
 * @fn BaseXmlBuilder::createXmlBaseDocument
 * @brief Creating a document.
 *
 * @param rootelement : root tag.
 */
//void BaseXmlBuilder::createXmlBaseDocument(QString rootelement)
//{
//    openFile(QIODevice::WriteOnly);
//    //delete older characters
//    clearFileText();

//    //set device
//    wxml->setDevice(file);

//    //seek specified line
//    wxml->setAutoFormatting(true);

//    //set 1 tabs
//    wxml->setAutoFormattingIndent(-1);

//    wxml->writeStartDocument();
//    wxml->writeTextElement(rootelement, endLineStr());
//    wxml->writeEndDocument();

//    closeFile();
//}

void BaseXmlBuilder::createXmlBaseDocument(QString rootelement, QString rootattr, QString rootattrvalue)
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
    wxml->writeStartElement(rootelement);

    if(rootattr.length() > 0){
        wxml->writeAttribute(rootattr, rootattrvalue);
    }

    wxml->writeCharacters(endLineStr());

    wxml->writeEndElement();
    wxml->writeEndDocument();

    closeFile();
}

/**
 * @fn BaseXmlBuilder::deleteElementGroup
 * @brief Delete XML block with the specified attribute value.
 *
 * @param element    : The name of the tag containing attr, value.
 * @param attr       : attribute name.
 * @param value      : The value of the attribute.
 * @param withparent : whether to leave tags attr, value.
 *
 * @return True if the process was successful
 * @remarks if it can't get specified element, this function does nothing.
 */
bool BaseXmlBuilder::deleteElementGroup(QString element, QString attr, int value, bool withparent)
{
    bool hasid = false;
    qint64 firstline = 0;
    qint64 endline = 0;
    QString deletedText;

    openFile(QIODevice::ReadWrite);
    openedFileReset();

    //get start and end line counter
    while (!(rxml->atEnd() || rxml->hasError()))
    {
        rxml->readNext();
        if (rxml->isStartElement()
                && rxml->name().toString() == element
                && rxml->attributes().value(attr).toInt() == value)
        {
            hasid = true;
            firstline = rxml->lineNumber();
        }

        if(rxml->isEndElement() && hasid && rxml->name() == element){
//            hasid = false;
            endline = rxml->lineNumber();
            break;
        }

    }

    qDebug() << "[BaseXmlBuilder::deleteElementGroup] firstline: " << firstline \
             << " endline: " << endline;

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
            deletedText.append(endLineStr());
        }

        //以上以下にすると外のタグだけは残る
        if(!withparent && (linecount <= firstline || linecount >= endline)){
            deletedText.append(tmp);
            deletedText.append(endLineStr());
        }
    }

    clearFileText();
//    qDebug() << deletedText;

    //add text
    del << deletedText;

    closeFile();
    return true;
}

/**
 * @fn BaseXmlBuilder::getElementFirstLineNumber
 * @brief Returns the first line of the tag block of the element whose value
 *        associated with attr matches.
 *
 * @param element : Tag name
 *
 * @return The first line of the tag block specified by element.
 *         If it does not have any elements, then returns -1.
 * @remarks get first line in multiple specified element.
 */
qint64 BaseXmlBuilder::getElementFirstLineNumber(QString element)
{
    qint64 line = 0;

    openFile(QIODevice::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
        if (rxml->readNext() == QXmlStreamReader::StartElement
            && rxml->name() == element)
        {
            line = rxml->lineNumber();
            break;
        }
    }

//    qDebug() << "[BaseXmlBuilder::getElementFirstLineNumber (1)] : " << element \
//                                    << attr << attrvalue << "linenum : " << line;
    checkXmlError();
    closeFile();
    return line;
}

/**
 * @fn BaseXmlBuilder::getElementFirstLineNumber
 * @brief Returns the first line of the tag block of the element whose value
 *        associated with attr matches.
 *
 * @param element   : Tag name
 * @param attr      : Attribute name in tag.
 * @param attrvalue : Value associated with the attribute.
 *
 * @return The first line of the tag block specified by element.
 *         If it does not have any elements, then returns -1.
 */
qint64 BaseXmlBuilder::getElementFirstLineNumber(QString element, QString attr, QString attrvalue)
{
    qint64 line = -1;

    openFile(QIODevice::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
        if (rxml->readNext() == QXmlStreamReader::StartElement
                && rxml->name() == element
                && rxml->attributes().value(attr) == attrvalue)
        {
            line = rxml->lineNumber();
            break;
        }
    }

//    qDebug() << "[BaseXmlBuilder::getElementFirstLineNumber (3)] : " << element \
//                                    << attr << attrvalue << "linenum : " << line;
    checkXmlError();
    closeFile();
    return line;
}

/**
 * @fn BaseXmlBuilder::getElementEndLineNumber
 * @brief Returns the last line of the tag block of the element whose value
 *        associated with attr matches.
 *
 * @param element : Tag name
 * @param attr    : Attribute name in tag.
 * @param value   : Value associated with the attribute.
 *
 * @return The last line of the tag block specified by element.
 */
qint64 BaseXmlBuilder::getElementEndLineNumber(QString element, QString attr, int value)
{
    qint64 line = 0;
    bool flags = false;

    openFile(QIODevice::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
        rxml->readNext();
        if (rxml->isStartElement() && rxml->name().toString() == element)
        {
            QString val = rxml->attributes().value(attr).toString();
            if(value == val.toInt()) flags = true;
        }

        if(flags && rxml->isEndElement() && rxml->name().toString() == element){
//            flags = false;
            line = rxml->lineNumber();
            break;
        }
    }

    checkXmlError();
    closeFile();
    return line;
}

/**
 * @fn BaseXmlBuilder::getElementItemsCount
 * @brief returns the number of tag blocks contained in element.
 *
 * @param element : tag name
 *
 * @return The number of tag blocks included in the tag.
 */
int BaseXmlBuilder::getElementItemsCount(QString element)
{
    int count = 0;
    openFile(QIODevice::ReadOnly);
    openedFileReset();

    while (!(rxml->atEnd() || rxml->hasError()))
    {
        if (rxml->readNext() == QXmlStreamReader::StartElement
            && rxml->name() == element)
        {
            count++;
        }
    }

    checkXmlError();
    closeFile();

    qDebug() << "[BaseXmlBuilder::getElementItemsCount] element: " << element \
             << " count: " << count;

    return count;
}

/**
 * @fn BaseXmlBuilder::appendTabIndent
 * @brief returns the number of tabs
 *
 * @param num : numbers of tab sequence
 *
 * @return numbers of tab sequence
 */
QString BaseXmlBuilder::appendTabIndent(int num)
{
    QString indent;
    for(int i = 0; i< num; i++){
        indent.append("\t");
    }
    return indent;
}

/**
 * @fn BaseXmlBuilder::checkXmlError
 * @brief xml file reading error check.
 */
void BaseXmlBuilder::checkXmlError(){
    if (rxml->hasError())
        qDebug() << "[BaseXmlBuilder::checkXmlError] : XML read error: " << rxml->errorString();
}

/**
 * @fn BaseXmlBuilder::openedFileReset
 * @brief Initialize the file reading position.
 */
void BaseXmlBuilder::openedFileReset(){

    //file seek to first point
    file->reset();

    //set new device
    rxml->setDevice(file);
}

/**
 * @fn BaseXmlBuilder::openFile
 * @brief Open file.
 *
 * @param flags : Qt designed file open flags.
 *
 * @return Whether the file was opened.
 */
bool BaseXmlBuilder::openFile(QFlags<QIODevice::OpenModeFlag> flags)
{
    if (!file->open(flags | QFile::Text)) {
        qDebug() << "[BaseXmlBuilder::openFile] : Cannot read file.";
        return false;
    }

    return true;
}

/**
 * @fn BaseXmlBuilder::getTabbedXmlString
 * @brief adjust code formatting. it becomes able to insert created xml.
 *
 * @param tmp    : The name of the temporary file.
 * @param indent : Number of tabs to add.
 *
 * @return String with tabs.
 */
QString BaseXmlBuilder::getTabbedXmlString(QTemporaryFile *tmp, int indent)
{
    if(tmp->open()){
        QTextStream in(tmp);
        QString endline = endLineStr();
        QString appenddata = in.readAll();

        QRegularExpression re;
        re.setPattern(endline);

        appenddata.replace( re, endline + appendTabIndent(indent));

        //qDebug() << appenddata;
        tmp->close();

        //delete first position \n code
        appenddata.remove(0,1);

        //append to last character
        appenddata.append(endline);

        return appenddata;
    }

    return nullptr;
}
