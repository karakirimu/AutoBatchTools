/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTCONVERTER_H
#define VARIANTCONVERTER_H

#include "variantconverter_global.h"
#include <QObject>

class VARIANTCONVERTERSHARED_EXPORT VariantConverter : public QObject
{
    Q_OBJECT
public:
    explicit VariantConverter(QObject *parent = nullptr);
    ~VariantConverter();

    static bool stringToBool(QString checked);
    static QString boolToString(bool checked);
};


#endif // VARIANTCONVERTER_H
