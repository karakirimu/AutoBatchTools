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

#ifndef SCHEDULERXMLCONSTANT_H
#define SCHEDULERXMLCONSTANT_H

#include <QString>

class SchedulerXmlConstant {
public:
    SchedulerXmlConstant(){}
    ~SchedulerXmlConstant(){}

    //! Root element name "startup"
    const QString TAG_ROOT          = "startup";

    //! Scheduler XML version code attribute "version"
    const QString ATTR_ROOT         = "version";

    //! Scheduler XML version "1.00"
    const QString ATTRVALUE_ROOT    = "1.00";

    //! Tags that include tags after this tag
    const QString TAG_FIRSTLAYER    = "item";

    //! "name"
    const QString TAG_NAME          = "name";

    //! "prof"
    const QString TAG_PROFILE       = "prof";

    //! "valid"
    const QString TAG_SCHEDULED     = "valid";

    //! "radio"
    const QString TAG_SCHEDULETYPE  = "radio";

    //! "schdt"
    const QString TAG_DATETIME      = "schdt";

    //! "schsec"
    const QString TAG_SECOND        = "schsec";

    //! "scht"
    const QString TAG_TIME          = "scht";

    //! "schday"
    const QString TAG_DATE          = "schday";

    //! unique removed (deprecated)
    const QString TAG_UNIQUE        = "unique";

    //!
    const QString ATTR_ITEM_ID_INT  = "id";

};

#endif // SCHEDULERXMLCONSTANT_H
