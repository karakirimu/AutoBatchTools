#include "basefilesearch.h"

#include <QDateTime>

BaseFileSearch::BaseFileSearch(QObject *parent) : QObject(parent)
{

}

BaseFileSearch::~BaseFileSearch()
{

}

QStringList BaseFileSearch::listFiles(QString path, QStringList *searchword)
{
    checkConditionData(searchword);

    QDirIterator dit(path, *searchword, QDir::Files);
    QStringList files;

    while (dit.hasNext()){
        files.append(dit.next());
    }
    return files;

}

QStringList BaseFileSearch::listFilesRecursive(QString path, QStringList *searchword)
{
    checkConditionData(searchword);

    QDirIterator dit(path, *searchword, QDir::Files, QDirIterator::Subdirectories);
    QStringList files;

    while (dit.hasNext()){
        files.append(dit.next());
    }
    return files;
}

void BaseFileSearch::setCurrentTimeCondition(QStringList *filelist, qint64 limitedtime)
{
    int filecount = filelist->count();
    QList<int> deleteddata;
    QString file;
    QDateTime dtime = QDateTime::currentDateTime();
    dtime.addSecs(-limitedtime);

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
        //older.secsto(newer) > 0
        if(dtime.secsTo(info.lastModified()) < 0) deleteddata.append(i);
    }

    deleteLists(&deleteddata, filelist);
}

void BaseFileSearch::setModifiedTimeCondition(QStringList *filelist, QDateTime basetime, int type)
{
    int filecount = filelist->count();
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
    int filecount = filelist->count();
    QList<int> deleteddata;
    QString file;

    //check delete files
    for(int i = 0; i < filecount; i++){
        file = filelist->at(i);
        QFileInfo info(file);
        conditionTime(&deleteddata, i, basetime.secsTo(info.created()), type);
    }

    deleteLists(&deleteddata, filelist);
}

void BaseFileSearch::setFilesizeCondition(QStringList *filelist, long long basebytes, int type)
{
    int filecount = filelist->count();
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
        if(targetbytes <= basebytes) deleteddata->append(counter);
        break;
    case 1:
        if(targetbytes < basebytes) deleteddata->append(counter);
        break;
    case 2:
        if(targetbytes != basebytes) deleteddata->append(counter);
        break;
    case 3:
        if(targetbytes > basebytes) deleteddata->append(counter);
        break;
    case 4:
        if(targetbytes >= basebytes) deleteddata->append(counter);
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
        if(time != 0) deleteddata->append(counter);
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
    int count = deletedindex->count();
    for(int i = count-1; i >= 0; i--){
        filelist->removeAt(deletedindex->at(i));
    }
}
