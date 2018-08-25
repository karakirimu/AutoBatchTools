#ifndef FILESEARCHLOADER_H
#define FILESEARCHLOADER_H

#include "filesearchloader_global.h"
#include <QDateTime>
#include "basefilesearch.h"
#include <../searchxmlbuilder/searchxmlbuilder.h>
#include <../variantconverter/variantconverter.h>


class FILESEARCHLOADERSHARED_EXPORT FileSearchLoader : public BaseFileSearch
{
public:
    explicit FileSearchLoader(QObject *parent = 0);
    ~FileSearchLoader();

    QStringList searchFromXml(int itemid);
    QStringList searchFromStrList(QList<QStringList> *list);
private:
    SearchXmlBuilder *builder;
//    enum{NAME, KEYWORD, DIR, RECURSIVE, SECONDS, CREATION, MODIFIED, FSIZE_1, FSIZE_2};

//    BaseFileSearch *finder;
    long long stringToLongLong(QString num);
};

#endif // FILESEARCHLOADER_H
