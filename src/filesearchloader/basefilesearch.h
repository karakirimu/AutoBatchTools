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
    ~BaseFileSearch();

    QStringList listFiles(QString path, QStringList *searchword);
    QStringList listFilesRecursive(QString path, QStringList *searchword);
    void setRegularExpressionCondition(QStringList *filelist, QString regexp);
    void setCurrentTimeCondition(QStringList *filelist, qint64 limitedtime);
    void setModifiedTimeCondition(QStringList *filelist, QDateTime basetime, int type);
    void setCreatedTimeCondition(QStringList *filelist, QDateTime basetime, int type);
    void setFilesizeCondition(QStringList *filelist, long long basebytes, int type);

private:
    void conditionFileSize(QList<int> *deleteddata, int counter, long long targetbytes, long long basebytes, int type);
    void conditionTime(QList<int> *deleteddata, int counter, qint64 time, int type);
    void checkConditionData(QStringList *searchword);
    void deleteLists(QList<int> *deletedindex, QStringList *filelist);
};

#endif // BASEFILESEARCH_H
