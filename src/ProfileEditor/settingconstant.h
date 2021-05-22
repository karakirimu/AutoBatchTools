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
    const QString HIDE_TOOLBARTEXT =       "abe_toolbar#hidetext";

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
    const QString ABE_PROCESS_HIDE_DESC =  "abe-process-hide-description";

    const QString ABE_INFO_BACKGROUND =      "abe-info-background";
    const QString ABE_INFO_TEXTCOLOR  =      "abe-info-textcolor";
    const QString ABE_EXEC_BACKGROUND =      "abe-exec-background";
    const QString ABE_EXEC_TEXTCOLOR  =      "abe-exec-textcolor";
    const QString ABE_SEARCH_BACKGROUND =    "abe-search-background";
    const QString ABE_SEARCH_TEXTCOLOR  =    "abe-search-textcolor";
    const QString ABE_PLUGIN_BACKGROUND =    "abe-plugin-background";
    const QString ABE_PLUGIN_TEXTCOLOR  =    "abe-plugin-textcolor";
    const QString ABE_PROFILE_BACKGROUND =   "abe-profile-background";
    const QString ABE_PROFILE_TEXTCOLOR  =   "abe-profile-textcolor";

    const QString ABE_INFO_BACKGROUND_DEFAULT =      "#f8f8f8";
    const QString ABE_INFO_TEXTCOLOR_DEFAULT  =      "#000000";
    const QString ABE_EXEC_BACKGROUND_DEFAULT =      "#e2f6ff";
    const QString ABE_EXEC_TEXTCOLOR_DEFAULT  =      "#000000";
    const QString ABE_SEARCH_BACKGROUND_DEFAULT =    "#dcedc8";
    const QString ABE_SEARCH_TEXTCOLOR_DEFAULT  =    "#000000";
    const QString ABE_PLUGIN_BACKGROUND_DEFAULT =    "#ffcdd2";
    const QString ABE_PLUGIN_TEXTCOLOR_DEFAULT  =    "#000000";
    const QString ABE_PROFILE_BACKGROUND_DEFAULT =   "#ffecb3";
    const QString ABE_PROFILE_TEXTCOLOR_DEFAULT  =   "#000000";

};

#endif // SETTINGCONSTANT_H
