/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef SETTINGCONSTANT_H
#define SETTINGCONSTANT_H

#include <QString>

class SettingConstant {
public:
    SettingConstant(){}
    ~SettingConstant(){}

    //! Output setting file path
    const QString OUTPUT_FILE_ABR =        "./settings.ini";

    const QString ABR_GEOMETRY =           "abr_geometry";
    const QString ABR_WINDOWSTATE =        "abr_windowstate";
    const QString ABR_PROFILECOMBO =       "abr_profilecombo";


    const QString GROUP_ABR =              "abr";

    const QString ABR_LANGUAGE =           "abr-language";
    const QString ABR_THEME =              "abr-theme";
    const QString ABR_FONT =               "abr-font";
    const QString ABR_FONTSIZE =           "abr-fontsize";

};

#endif // SETTINGCONSTANT_H
