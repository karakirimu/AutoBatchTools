/*
 * Copyright 2016-2020 karakirimu
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
