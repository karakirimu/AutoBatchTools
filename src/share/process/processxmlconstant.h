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

#ifndef PROCESSXMLCONSTANT_H
#define PROCESSXMLCONSTANT_H

#include <QString>

class ProcessXmlConstant {
public:
    //! Root element name "profile"
    const QString TAG_ROOT                    = "profile";

    //! ProfileEditor XML version code attribute "version"
    const QString ATTR_ROOT                   = "version";

    //! ProfileEditor XML version "1.00"
    const QString ATTRVALUE_ROOT              = "1.00";

    //! Tags that include tags after this tag
    const QString TAG_FIRSTLAYER              = "order";

    //! Shared Tag
    const QString TAG_TYPE                    = "type";

    //! UI Selected Data Type Tag
    const QString TAG_FUNCTIONSELECT          = "istack";

    //! Information Tag (HA1 : Has Attribute 1)
    const QString TAG_I_NAME                  = "iname";
    const QString TAG_I_VERSION               = "ver";
    const QString TAG_I_AUTHOR                = "author";
    const QString TAG_I_DESCRIPTION           = "desc";
    const QString TAG_I_FILEINPUT_BOOL        = "finput";
    const QString TAG_I_FILEINPUT_SEARCH_BOOL = "sinput";
    const QString TAG_I_FILESEARCH_NAME       = "fsname";
    const QString TAG_I_PROCESS_BOOL_HA1      = "rloop";
    const QString TAG_I_ARG_IN_ONELOOP_INT    = "rlarg";
    const QString TAG_I_RECURSIVE_LOOPMAX_INT = "reloop";
    const QString TAG_I_PROFILE_BASEPATH      = "basepath";

    //! Search Tag
    const QString TAG_FS_NAME_HA1             = "sname";
    const QString TAG_FS_SEPARATOR            = "sep";
    const QString TAG_FS_VARIANT              = "var";
    const QString TAG_FS_FILEPATH_HA1         = "output";
    const QString TAG_FS_WRITEOPTION_INT      = "fitype";

    //! profileload Tag
    const QString TAG_PLOAD_FILEPATH          = "prfile";

    //! Executable Tag
    const QString TAG_E_TIMEOUT_BOOL_HA1      = "timeout";
    const QString TAG_E_DETACH_BOOL           = "detach";
    const QString TAG_E_CMD_HA1               = "exc";
    const QString TAG_E_COMMANDCOUNT_INT      = "cmdc";

    //! Plugin Tag
    const QString TAG_P_NAME                  = "plname";
    const QString TAG_P_FILEPATH              = "plfile";
    const QString TAG_P_CMD_HA1               = "plc";
    const QString TAG_P_COMMANDCOUNT_INT      = "pcmdc";

    //! Local Tag
    const QString TAG_L_VARIANT_HA1           = "lvar";
    const QString TAG_L_VARIANTCOUNT_INT      = "localc";

    //! Attribute define (index 2)
    const QString ATTR_ONLY_SCHEDULER_BOOL    = "only";
    const QString ATTR_COMMAND_ID_INT         = "id";
    const QString ATTR_FS_OUTPUTOPTION_INT    = "radio";
    const QString ATTR_I_PROCESSMAX_INT       = "max";

    //! Attribute executable
    const QString ATTR_TIMEOUT_INT            = "dur";

    //! Attribute local
    const QString ATTR_L_VALUE                = "lval";
};

#endif // PROCESSXMLCONSTANT_H
