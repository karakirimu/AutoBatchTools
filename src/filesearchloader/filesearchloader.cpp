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
    QString keyword = list->at(KEYWORD).at(1);

    //split keywords
    QStringList keywordlist = keyword.split(" ");

    //dir convert
    QFileInfo dirinfo(list->at(DIR).at(1));

    //get file lists
    if(VariantConverter::stringToBool(list->at(RECURSIVE).at(1))){
        result = listFilesRecursive(dirinfo.canonicalFilePath(), &keywordlist);
    }else{
        result = listFiles(dirinfo.canonicalFilePath(), &keywordlist);
    }

    //filtering data from currenttime
    if(VariantConverter::stringToBool(list->at(SECONDS).at(3))){
        setCurrentTimeCondition(&result, VariantConverter::stringToInt(list->at(SECONDS).at(1)));
    }

    //filtering data from createdtime
    if(VariantConverter::stringToBool(list->at(CREATION).at(3))){
        QDateTime dtime = QDateTime::fromString(list->at(CREATION).at(1), "yyyy/MM/dd HH:mm:ss");
        setCreatedTimeCondition(&result, dtime, VariantConverter::stringToInt(list->at(CREATION).at(5)));
    }

    //filtering data from modifiedtime
    if(VariantConverter::stringToBool(list->at(MODIFIED).at(3))){
        QDateTime time = QDateTime::fromString(list->at(MODIFIED).at(1), "yyyy/MM/dd HH:mm:ss");
        setModifiedTimeCondition(&result, time, VariantConverter::stringToInt(list->at(MODIFIED).at(5)));
    }

    //filtering data from filesize1
    if(VariantConverter::stringToBool(list->at(FSIZE_1).at(3))){
        setFilesizeCondition(&result, stringToLongLong(list->at(FSIZE_1).at(1)), VariantConverter::stringToInt(list->at(FSIZE_1).at(5)));
    }

    //filtering data from filesize2
    if(VariantConverter::stringToBool(list->at(FSIZE_2).at(3))){
        setFilesizeCondition(&result, stringToLongLong(list->at(FSIZE_2).at(1)), VariantConverter::stringToInt(list->at(FSIZE_2).at(5)));
    }

    return result;
}

long long FileSearchLoader::stringToLongLong(QString num)
{
    return num.toLongLong();
}
