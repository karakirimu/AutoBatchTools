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
