#ifndef VARIANTCONVERTER_H
#define VARIANTCONVERTER_H

#include "variantconverter_global.h"
#include <QObject>

class VARIANTCONVERTERSHARED_EXPORT VariantConverter : public QObject
{
    Q_OBJECT
public:
    explicit VariantConverter(QObject *parent = 0);
    ~VariantConverter();

    static int stringToInt(QString data);
    static int boolToInt(bool data);

    static bool stringToBool(QString checked);
    static bool intToBool(int data);

    static QString boolToString(bool checked);
};


#endif // VARIANTCONVERTER_H
