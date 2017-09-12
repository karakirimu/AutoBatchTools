#ifndef BASEFILESEARCH_H
#define BASEFILESEARCH_H

#include "filesearchloader_global.h"
#include <QObject>
#include <QDebug>
#include <QDirIterator>

class FILESEARCHLOADERSHARED_EXPORT BaseFileSearch : public QObject
{
    Q_OBJECT
public:
    FILESEARCHLOADERSHARED_EXPORT explicit BaseFileSearch(QObject *parent = 0);
    FILESEARCHLOADERSHARED_EXPORT ~BaseFileSearch();

    FILESEARCHLOADERSHARED_EXPORT QStringList listFiles(QString path, QStringList *searchword);
    FILESEARCHLOADERSHARED_EXPORT QStringList listFilesRecursive(QString path, QStringList *searchword);
    FILESEARCHLOADERSHARED_EXPORT void setCurrentTimeCondition(QStringList *filelist, qint64 limitedtime);
    FILESEARCHLOADERSHARED_EXPORT void setModifiedTimeCondition(QStringList *filelist, QDateTime basetime, int type);
    FILESEARCHLOADERSHARED_EXPORT void setCreatedTimeCondition(QStringList *filelist, QDateTime basetime, int type);
    FILESEARCHLOADERSHARED_EXPORT void setFilesizeCondition(QStringList *filelist, long long basebytes, int type);

private:
    void conditionFileSize(QList<int> *deleteddata, int counter, long long targetbytes, long long basebytes, int type);
    void conditionTime(QList<int> *deleteddata, int counter, qint64 time, int type);
    void checkConditionData(QStringList *searchword);
    void deleteLists(QList<int> *deletedindex, QStringList *filelist);
};

#endif // BASEFILESEARCH_H
