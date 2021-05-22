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
    const QString OUTPUT_FILE_ABS =        "./settings.ini";

    const QString GROUP_ABS =              "abs";

    const QString ABS_LANGUAGE =           "abs-language";

    const QString ABS_MINIMIZE =           "abs-message-minimize";
    const QString ABS_MINIMIZE_MS =        "abs-message-minimize-ms";

    const QString ABS_TIMERSTART =         "abs-message-timer-start";
    const QString ABS_TIMERSTART_MS =      "abs-message-timer-start-ms";
    const QString ABS_TIMERSTOP =          "abs-message-timer-stop";
    const QString ABS_TIMERSTOP_MS =       "abs-message-timer-stop-ms";

    const QString ABS_TASKSTART =          "abs-message-task-start";
    const QString ABS_TASKSTART_MS =       "abs-message-task-start-ms";
    const QString ABS_TASKFINISHED =       "abs-message-task-end";
    const QString ABS_TASKFINISHED_MS =    "abs-message-task-end-ms";

    const QString ABS_TASKUNSELECTED =     "abs-message-task-unselect";
    const QString ABS_TASKUNSELECTED_MS =  "abs-message-task-unselect-ms";

    const QString ABS_MINIMIZE_WINDOW =    "abs-window-minimize";

    const QString ABS_THEME =              "abs-theme";
    const QString ABS_FONT =               "abs-font";
    const QString ABS_FONTSIZE =           "abs-fontsize";

};

#endif // SETTINGCONSTANT_H
