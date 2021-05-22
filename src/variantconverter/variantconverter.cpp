/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
