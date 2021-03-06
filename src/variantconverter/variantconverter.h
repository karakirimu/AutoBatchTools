/*
 * Copyright 2016-2021 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
