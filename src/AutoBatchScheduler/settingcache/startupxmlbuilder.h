/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#ifndef STARTUPXMLBUILDER_H
#define STARTUPXMLBUILDER_H

#include <settingcache/schedulerxmlconstant.h>
#include <../xmlbuilder/xmlbuilder.h>
/**
 * @class StartupSettings_xml
 * @brief QList<QStringList> short description
 * @details this function sets text and checkbox.
 * data structure
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 * \    :        0:      1:
 * 0    :name     :text   :
 * 1    :prof     :text   :
 * 2    :valid    :text   :
 * 3    :radio    :text   ://radio button position
 * 4    :schdt    :text   ://schedule datetime
 * 5    :schsec   :text   ://schedule seconds
 * 6    :scht     :text   ://schedule time
 * 7    :schday   :text   ://schedule day
 * 8    :unique   :text   ://tray managing number
 *
 * schday data structure
 * weeks: 1 or bigger than 1 (0 is invalid)
 * day : enabled = 1, disabled = 0
 * weeks every :monday :tuesday :wednesday :thursday :friday :saturday :sunday :
 *
 * example:
 * 20011010
 * 2:0:0:1:1:0:1:0:
 * every weeks = 2, mon = 0, tue = 0, wed = 1, thu = 1, fri = 0, sat = 1, sun = 0
 */

class StartupXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    explicit StartupXmlBuilder(QObject *parent = nullptr);
    virtual ~StartupXmlBuilder() override;

    bool readAll(QList<QList<QStringList> *> *itemlist);
    bool writeAll(const QList<QList<QStringList> *> *itemlist);

    bool readItem(int itemid, QList<QStringList> *itemlist);   
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    void createDocument();

    enum{ONESHOT,WEEKLOOP,TIMELOOP};
    enum{NAME,PROF,VALID,SELECTEDTYPE,SC_DATETIME,SC_SECONDS,SC_TIME,SC_DAY,UNIQUE};

private:
    int count();
    void setSearchItemData(QString element, QList<QStringList> *list) override;

    SchedulerXmlConstant sxc;
};

#endif // STARTUPXMLBUILDER_H
