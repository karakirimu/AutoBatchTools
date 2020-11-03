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

class SchedulerXmlConstant {
public:
    //! Root element name "startup"
    static constexpr char TAG_ROOT[]          = "startup";

    //! Scheduler XML version code attribute "version"
    static constexpr char ATTR_ROOT[]         = "version";

    //! Scheduler XML version "1.00"
    static constexpr char ATTRVALUE_ROOT[]    = "1.00";

    //! Tags that include tags after this tag
    static constexpr char TAG_FIRSTLAYER[]    = "item";

    //! "name"
    static constexpr char TAG_NAME[]          = "name";

    //! "prof"
    static constexpr char TAG_PROFILE[]       = "prof";

    //! "valid"
    static constexpr char TAG_SCHEDULED[]     = "valid";

    //! "radio"
    static constexpr char TAG_SCHEDULETYPE[]  = "radio";

    //! "schdt"
    static constexpr char TAG_DATETIME[]      = "schdt";

    //! "schsec"
    static constexpr char TAG_SECOND[]        = "schsec";

    //! "scht"
    static constexpr char TAG_TIME[]          = "scht";

    //! "schday"
    static constexpr char TAG_DATE[]          = "schday";

    //! unique removed (deprecated)
    static constexpr char TAG_UNIQUE[]        = "unique";

    //!
    static constexpr char ATTR_ITEM_ID_INT[]  = "id";

};

#endif // SCHEDULERXMLCONSTANT_H
