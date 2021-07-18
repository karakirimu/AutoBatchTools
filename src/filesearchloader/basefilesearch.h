/*
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef BASEFILESEARCH_H
#define BASEFILESEARCH_H

#include "filesearchloader_global.h"
#include <QObject>
#include <QDebug>
#include <QDirIterator>
#include <QRegularExpression>

class FILESEARCHLOADERSHARED_EXPORT BaseFileSearch : public QObject
{
    Q_OBJECT
public:
    explicit BaseFileSearch(QObject *parent = nullptr);
    virtual ~BaseFileSearch() override;

    QStringList listFiles(QString path, QStringList *searchword);
    QStringList listFilesRecursive(QString path, QStringList *searchword);
    void setRegularExpressionCondition(QStringList *filelist, QString regexp);
    void setCurrentTimeCondition(QStringList *filelist, qint64 limitedtime);

    void setModifiedTimeCondition(QStringList *filelist
                                  , QDateTime basetime
                                  , int type);
    void setCreatedTimeCondition(QStringList *filelist
                                 , QDateTime basetime
                                 , int type);
    void setFilesizeCondition(QStringList *filelist
                              , long long basebytes
                              , int type);

//    enum class SizeConditionType : int {
//        Greater,
//        GreaterOrEqual,
//        Equal,
//        LessOrEqual,
//        Less
//    };

private:
    void conditionFileSize(QList<int> *deleteddata
                           , int counter
                           , long long targetbytes
                           , long long basebytes
                           , int type);

    void conditionTime(QList<int> *deleteddata
                       , int counter
                       , qint64 time
                       , int type);

    void checkConditionData(QStringList *searchword);
    void deleteLists(QList<int> *deletedindex, QStringList *filelist);
};

#endif // BASEFILESEARCH_H
