/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
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
