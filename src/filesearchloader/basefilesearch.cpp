/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "basefilesearch.h"

#include <QDateTime>

BaseFileSearch::BaseFileSearch(QObject *parent)
    : QObject(parent)
{

}

BaseFileSearch::~BaseFileSearch()
{

}

QStringList BaseFileSearch::listFiles(QString path
                                      , QStringList *searchword)
{
    checkConditionData(searchword);

    QDirIterator dit(path, *searchword, QDir::Files);
    QStringList files;

    while (dit.hasNext()){
        files.append(dit.next());
    }
    return files;

}

QStringList BaseFileSearch::listFilesRecursive(QString path
                                               , QStringList *searchword)
{
    checkConditionData(searchword);

    QDirIterator dit(path, *searchword, QDir::Files, QDirIterator::Subdirectories);
    QStringList files;

    while (dit.hasNext()){
        files.append(dit.next());
    }
    return files;
}

/**
 * @fn BaseFileSearch::setRegularExpressionCondition
 * @brief Filter filelist by a regular expression.
 * @param filelist    List of files searched by listFiles... functions.
 * @param regexp      Regular expression string
 */
void BaseFileSearch::setRegularExpressionCondition(QStringList *filelist, QString regexp)
{
    qsizetype filecount = filelist->count();
    QList<int> deleteddata;

    for(int i = 0; i < filecount; i++){
        if(!static_cast<QString>(filelist->at(i)).contains(QRegularExpression(regexp))){
            //not match
            deleteddata.append(i);
        }
    }

    deleteLists(&deleteddata, filelist);
}

/**
 * @fn BaseFileSearch::setCurrentTimeCondition
 * @brief Extract files created within x seconds from current time from "filelist".
 * @param filelist          List of files searched by listFiles... functions.
 * @param limitedtime       The time x seconds before the current time.
 */
void BaseFileSearch::setCurrentTimeCondition(QStringList *filelist, qint64 limitedtime)
{
    qsizetype filecount = filelist->count();
    QList<int> deleteddata;
    QString file;
    QDateTime dtime = QDateTime::currentDateTime();

    // addsecs returns calculated Time
    dtime = dtime.addSecs(-limitedtime);

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
#ifdef QT_DEBUG
        qDebug() << "[BaseFileSearch::setCurrentTimeCondition] lastModified : " << info.lastModified();
        qDebug() << "[BaseFileSearch::setCurrentTimeCondition] birthtime  : " << info.birthTime();
        qDebug() << "[BaseFileSearch::setCurrentTimeCondition] setdatetime  : " << dtime;
        qDebug() << "[BaseFileSearch::setCurrentTimeCondition] calculated  : " << dtime.secsTo(info.lastModified());
#endif
        if(dtime.secsTo(info.lastModified()) < 0) deleteddata.append(i);
    }

    deleteLists(&deleteddata, filelist);
}

void BaseFileSearch::setModifiedTimeCondition(QStringList *filelist, QDateTime basetime, int type)
{
    qsizetype filecount = filelist->count();
    QList<int> deleteddata;
    QString file;

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
        conditionTime(&deleteddata, i, basetime.secsTo(info.lastModified()), type);
    }

    deleteLists(&deleteddata, filelist);
}

void BaseFileSearch::setCreatedTimeCondition(QStringList *filelist, QDateTime basetime, int type)
{
    qsizetype filecount = filelist->count();
    QList<int> deleteddata;
    QString file;

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
        conditionTime(&deleteddata, i, basetime.secsTo(info.birthTime()), type);
    }

    deleteLists(&deleteddata, filelist);
}

void BaseFileSearch::setFilesizeCondition(QStringList *filelist, long long basebytes, int type)
{
    qsizetype filecount = filelist->count();
    QList<int> deleteddata;
    QString file;

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
        conditionFileSize(&deleteddata, i, info.size(), basebytes, type);
    }

    deleteLists(&deleteddata, filelist);
}

/**
 * @brief BaseFileSearch::conditionFileSize
 * @param deleteddata
 * @param counter
 * @param targetbytes
 * @param basebytes
 * @param type
 *
 * type infomation
 * 0:greater than bytes
 * 1:greater than or equal to bytes
 * 2:equal to bytes
 * 3:less than or equal to bytes
 * 4:less than bytes
 *
 */
void BaseFileSearch::conditionFileSize(QList<int> *deleteddata
                                       , int counter
                                       , long long targetbytes
                                       , long long basebytes
                                       , int type)
{
    switch(type){
    case 0:
        if(targetbytes < basebytes) deleteddata->append(counter);
        break;
    case 1:
        if(targetbytes <= basebytes) deleteddata->append(counter);
        break;
    case 2:
        if(targetbytes == basebytes) deleteddata->append(counter);
        break;
    case 3:
        if(targetbytes >= basebytes) deleteddata->append(counter);
        break;
    case 4:
        if(targetbytes > basebytes) deleteddata->append(counter);
        break;
    default:
        break;
    }
}

/**
 * @brief BaseFileSearch::conditionTime
 * @param deleteddata
 * @param counter
 * @param time
 * @param type
 *
 * type infomation
 * 0:after
 * 1:equals to
 * 2:before
 */
void BaseFileSearch::conditionTime(QList<int> *deleteddata, int counter, qint64 time, int type)
{
    //older.secsTo(newer) > 0
    switch(type){
    case 0:
        if(time <= 0) deleteddata->append(counter);
        break;
    case 1:
        if(time == 0) deleteddata->append(counter);
        break;
    case 2:
        if(time >= 0) deleteddata->append(counter);
        break;
    default:
        break;
    }
}

void BaseFileSearch::checkConditionData(QStringList *searchword)
{
    if (searchword->isEmpty()){
        searchword->append("*");
    }

    if(searchword->first() == ""){
        searchword->clear();
        searchword->append("*");
    }
}

void BaseFileSearch::deleteLists(QList<int> *deletedindex, QStringList *filelist)
{
    //delete lists
    qsizetype count = deletedindex->count();
    for(qsizetype i = count - 1; i >= 0; i--){
        filelist->removeAt(deletedindex->at(i));
    }
}
