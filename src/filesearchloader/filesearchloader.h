#ifndef FILESEARCHLOADER_H
#define FILESEARCHLOADER_H

#include "filesearchloader_global.h"
#include "basefilesearch.h"
#include <QDateTime>
#include <../searchxmlbuilder/searchxmlbuilder.h>
#include <../variantconverter/variantconverter.h>


class FILESEARCHLOADERSHARED_EXPORT FileSearchLoader : public BaseFileSearch
{
    Q_OBJECT
public:
    FILESEARCHLOADERSHARED_EXPORT explicit FileSearchLoader(QObject *parent = 0);
    FILESEARCHLOADERSHARED_EXPORT ~FileSearchLoader();

    FILESEARCHLOADERSHARED_EXPORT QStringList searchFromXml(int itemid);
    FILESEARCHLOADERSHARED_EXPORT QStringList searchFromStrList(QList<QStringList> *list);
private:
    SearchXmlBuilder *builder;
    enum{NAME, KEYWORD, DIR, RECURSIVE, SECONDS, CREATION, MODIFIED, FSIZE_1, FSIZE_2};

//    BaseFileSearch *finder;
    long long stringToLongLong(QString num);
};

#endif // FILESEARCHLOADER_H
