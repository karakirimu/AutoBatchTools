#include "variantconverter.h"

VariantConverter::VariantConverter(QObject *parent) : QObject(parent){}
VariantConverter::~VariantConverter(){}

int VariantConverter::stringToInt(QString data)
{
    return data.toInt();
}

int VariantConverter::boolToInt(bool data)
{
    return data ? 1 : 0;
}

bool VariantConverter::stringToBool(QString checked)
{
    return checked == "yes" ? true : false;
}

bool VariantConverter::intToBool(int data)
{
    return (data == 1) ? true : false;
}

QString VariantConverter::boolToString(bool checked)
{
    return checked ? "yes" : "no";
}
