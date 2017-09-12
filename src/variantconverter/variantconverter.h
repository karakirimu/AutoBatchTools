#ifndef VARIANTCONVERTER_H
#define VARIANTCONVERTER_H

#include "variantconverter_global.h"
#include <QObject>

class VARIANTCONVERTERSHARED_EXPORT VariantConverter : public QObject
{
    Q_OBJECT
public:
    VARIANTCONVERTERSHARED_EXPORT explicit VariantConverter(QObject *parent = 0);
    VARIANTCONVERTERSHARED_EXPORT ~VariantConverter();

    VARIANTCONVERTERSHARED_EXPORT static int stringToInt(QString data);
    VARIANTCONVERTERSHARED_EXPORT static int boolToInt(bool data);

    VARIANTCONVERTERSHARED_EXPORT static bool stringToBool(QString checked);
    VARIANTCONVERTERSHARED_EXPORT static bool intToBool(int data);

    VARIANTCONVERTERSHARED_EXPORT static QString boolToString(bool checked);
};


#endif // VARIANTCONVERTER_H
