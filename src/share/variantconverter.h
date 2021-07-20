/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANTCONVERTER_H
#define VARIANTCONVERTER_H

#include <QString>

class VariantConverter
{
public:
    VariantConverter();
    virtual ~VariantConverter();

    /**
     * @fn VariantConverter::stringToBool
     * @brief Converts "yes" and "no" strings to bool.
     *
     * @param checked : strings.
     *
     * @return true if yes, false if no.
     */
    static inline bool stringToBool(QString checked){
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
    static inline QString boolToString(bool checked){
        return checked ? "yes" : "no";
    }
};


#endif // VARIANTCONVERTER_H
