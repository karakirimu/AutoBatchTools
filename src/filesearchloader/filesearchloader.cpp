#include "filesearchloader.h"

FileSearchLoader::FileSearchLoader(QObject *parent) : BaseFileSearch(parent)
{
    builder = new SearchXmlBuilder();
//    finder = new BaseFileSearch();
}

FileSearchLoader::~FileSearchLoader()
{
    delete builder;
//    delete finder;
}

QStringList FileSearchLoader::searchFromXml(int itemid)
{
    QList<QStringList> list;
    builder->readItem(itemid, &list);
    return searchFromStrList(&list);
}

QStringList FileSearchLoader::searchFromStrList(QList<QStringList> *list)
{
    QStringList result;

    //get keywords
    QString keyword = builder->fetch(SEARCH_KEYWORD, SEARCH_NONE, list);

    //split keywords
    QStringList keywordlist = keyword.split(" ");

    //dir convert
    QFileInfo dirinfo(builder->fetch(SEARCH_DIR, SEARCH_NONE, list));

    //get file lists
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_RECURSIVE, SEARCH_NONE, list))){
        result = listFilesRecursive(dirinfo.canonicalFilePath(), &keywordlist);
    }else{
        result = listFiles(dirinfo.canonicalFilePath(), &keywordlist);
    }

    //filtering list from regularexpression
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_REGEX, ENABLED, list))){
        setRegularExpressionCondition(&result, builder->fetch(SEARCH_REGEX, SEARCH_NONE, list));
    }

    //filtering list from currenttime
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_SECONDS, ENABLED, list))){
        setCurrentTimeCondition(&result, (builder->fetch(SEARCH_SECONDS, SEARCH_NONE, list)).toInt());
    }

    //filtering data from createdtime
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_CREATION, ENABLED, list))){
        QDateTime dtime = QDateTime::fromString(builder->fetch(SEARCH_CREATION, SEARCH_NONE, list), "yyyy/MM/dd HH:mm:ss");
        setCreatedTimeCondition(&result, dtime, (builder->fetch(SEARCH_CREATION, COMBO, list)).toInt());
    }

    //filtering data from modifiedtime
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_MODIFIED, ENABLED, list))){
        QDateTime time = QDateTime::fromString(builder->fetch(SEARCH_MODIFIED, SEARCH_NONE, list), "yyyy/MM/dd HH:mm:ss");
        setModifiedTimeCondition(&result, time, (builder->fetch(SEARCH_MODIFIED, COMBO, list)).toInt());
    }

    //filtering data from filesize1
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_FSIZE_1, ENABLED, list))){
        setFilesizeCondition(&result, stringToLongLong(builder->fetch(SEARCH_FSIZE_1, SEARCH_NONE, list)) \
                             , (builder->fetch(SEARCH_FSIZE_1, COMBO, list)).toInt());
    }

    //filtering data from filesize2
    if(VariantConverter::stringToBool(builder->fetch(SEARCH_FSIZE_2, ENABLED, list))){
        setFilesizeCondition(&result, stringToLongLong(builder->fetch(SEARCH_FSIZE_2, SEARCH_NONE, list)) \
                             , (builder->fetch(SEARCH_FSIZE_2, COMBO, list)).toInt());
    }

    return result;
}

long long FileSearchLoader::stringToLongLong(QString num)
{
    return num.toLongLong();
}
