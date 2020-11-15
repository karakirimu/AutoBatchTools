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

#ifndef STARTUPXMLBUILDER_H
#define STARTUPXMLBUILDER_H

#include <settingcache/SchedulerXmlConstant.h>
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
    ~StartupXmlBuilder();

    bool readAll(QList<QList<QStringList> *> *itemlist);
    bool writeAll(const QList<QList<QStringList> *> *itemlist);

    bool readItem(int itemid, QList<QStringList> *itemlist);   
    bool addItem(const QList<QStringList> *itemlist);
    bool deleteItem(int itemid);
    bool editItem(int itemid, const QList<QStringList> *itemlist);
    void swapItem(int beforeitemid, int afteritemid);
    void copyItem(int itemid);
    bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    void createDocument();
    int count();

    enum{ONESHOT,WEEKLOOP,TIMELOOP};
    enum{NAME,PROF,VALID,SELECTEDTYPE,SC_DATETIME,SC_SECONDS,SC_TIME,SC_DAY,UNIQUE};

private:
    void setSearchItemData(QString element, QList<QStringList> *list);

//    const QString ROOTELEMENT = "startup";
//    const QString FIRSTLAYER = "item";
//    const QString ATTR = "id";
    SchedulerXmlConstant sxc;
};

#endif // STARTUPXMLBUILDER_H
