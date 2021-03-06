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

#include "variantconverter.h"

VariantConverter::VariantConverter(QObject *parent) : QObject(parent){}
VariantConverter::~VariantConverter(){}

/**
 * @fn VariantConverter::stringToBool
 * @brief Converts "yes" and "no" strings to bool.
 *
 * @param checked : strings.
 *
 * @return true if yes, false if no.
 */
bool VariantConverter::stringToBool(QString checked)
{
    return checked == "yes" ? true : false;
}

/**
 * @fn VariantConverter::boolToString
 * @brief Converts bool to "yes" and "no" strings.
 *
 * @param checked : bool value
 *
 * @return "yes" or "no", on the string.
 */
QString VariantConverter::boolToString(bool checked)
{
    return checked ? "yes" : "no";
}
