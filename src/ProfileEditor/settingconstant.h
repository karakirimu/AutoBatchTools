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
    const QString OUTPUT_FILE =            "./settings.ini";

    const QString GEOMETRY =               "abe_geometry";
    const QString WINDOW_STATE =           "abe_windowstate";


    const QString SHOW_TOOLBAR_EDIT =      "abe_toolbar#edit";
    const QString SHOW_TOOLBAR_RUN =       "abe_toolbar#run";
    const QString SHOW_TOOLBAR_RANGE =     "abe_toolbar#test#range";


    const QString SHOW_PROCESS =           "abe_dock#process";
    const QString SHOW_VARIANT =           "abe_dock#variant";
    const QString SHOW_TEST =              "abe_dock#runset";
    const QString SHOW_CONSOLE =           "abe_dock#console";
    const QString HIDE_TITLEBAR =          "abe_dock#autohide";

    const QString GROUP_ABE =              "abe";
    const QString ABE_LANGUAGE =           "abe-language";
    const QString ABE_RECENTLY_OPENED =    "abe-lastopened";
    const QString ABE_RECENT_FILES =       "abe-recentfiles";
    const QString ABE_LAST_SAVED_DIR =     "abe-lastsaved";
    const QString ABE_THEME =              "abe-theme";
    const QString ABE_FONT =               "abe-font";
    const QString ABE_FONTSIZE =           "abe-fontsize";
    const QString ABE_AUTOSAVE_DIR =       "abe-tempdir";
    const QString ABE_AUTOSAVE_PERIOD =    "abe-autosaveperiod";
    const QString ABE_RUNAS_SCHEDULER =    "abe-test-runas";

};

#endif // SETTINGCONSTANT_H
