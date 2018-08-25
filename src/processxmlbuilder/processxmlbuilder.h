#ifndef PROCESSXMLBUILDER_H
#define PROCESSXMLBUILDER_H

#include "processxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>

/**
 * @class profile*_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details
 *
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 *
 * data structure <order> type=info
 * \    :        0:      1:
 * 0    :type     :info   :
 * 1    :name     :text   :
 * 2    :ver      :text   :
 * 3    :author   :text   :
 * 4    :desc     :text   :
 * 5    :uptime   :text   :
 *
 * data structure <order> type=info (2018/07/07 update)
 * \    :        0:      1:      2:      3:
 * 0    :type     :info   :
 * 1    :name     :text   :
 * 2    :ver      :text   :
 * 3    :author   :text   :
 * 4    :desc     :text   :
 * 5    :finput   :yes/no :
 * 6    :sinput   :yes/no :
 * 7    :fsname   :text   :id     :data   :
 * 8    :rloop    :yes/no :max    :count  :
 * 9    :rlarg    :counts :
 * 10   :reloop   :counts :
 *
 *  data structure <order> type=local
 * \    :        0:      1:      2:      3:
 * 0    :type     :local  :
 * 1    :localc   :counts :
 * 2    :lvar     :text   :lval   :data   :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * data structure <order> type=normal
 * \    :        0:      1:      2:      3:
 * 0    :type     :normal :only   :data   :
 * 1    :nowait   :data   :
 * 2    :cmdc     :counts : //unnessesary?
 * 3    :cmd      :text   :id     :data   :
 * 4                    *
 * 5                    *
 * 6                    *
 * 7         it follows to the end.
 *
 * data structure <order> type=search
 * \    :        0:      1:      2:      3:
 * 0    :type     :search :only   :data   :
 * 1    :sname    :text   :id     :data   :
 * 2    :sep      :text   :
 * 3    :var      :text   :
 * 4    :output   :text   :radio  :data   :
 *
 * data structure <order> type=script
 * \    :        0:      1:      2:      3:
 * 0    :type     :script :only   :data   :
 * 1    :name     :text   :
 * 2    :var      :text   :
 * 3    :cmdc     :counts : //unnessesary?
 * 4    :cmd      :text   :id     :data   :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * data structure <order> type=other(profile)
 * \    :        0:      1:      2:      3:
 * 0    :type     :other  :only   :data   :
 * 1    :name     :text   :
 * 2    :file     :text   :
 *
 * data structure <order> type=temp(saved all current data)(except info)(2017/8/23 obsoleted)
 * \    \    :        0:      1:      2:      3:
 * 0    0    :type     :temp   :only   :data   :
 * -    1    :fstack   :data   ://first stack
 * 1    2    :istack   :data   ://inner stack
 * -    -    :type     :info   :
 * -    3    :name     :text   :
 * -    4    :ver      :text   :
 * -    5    :author   :text   :
 * -    6    :desc     :text   :
 * -    7    :uptime   :text   :
 * -    -    :type     :normal :
 * 2    8    :nowait   :data   :
 * 3    9    :cmdc     :counts :
 * -    -    :cmd      :text   :id     :data   :
 * -    -                    *
 * -    -                    *
 * -    -                    *
 * -    -         it follows to the end.
 * -    -    :type     :search :
 * 4+   10+  :sname    :text   :id     :data   :
 * 5+   11+  :sep      :text   :
 * 6+   12+  :var      :text   :
 * 7+   13+  :output   :text   :radio  :data   :
 * -    -    :type     :script :
 * 8+   14+  :name     :text   :
 * 9+   15+  :var      :text   :
 * 10+  15+  :cmdc     :counts :
 * -    -    :cmd      :text   :id     :data   :
 * -    -                    *
 * -    -                    *
 * -    -                    *
 * -    -         it follows to the end.
 * -    -    :type     :other  :
 * 11++ 16++ :name     :text   :
 * 12++ 17++ :file     :text   :
 *
 * * data structure <order> type=temp(saved all current data)(except info)(2017/8/23 revised)
 * \    :        0:      1:      2:      3:
 * 0    :type     :temp   :only   :data   :
 * 1    :istack   :data   ://inner stack
 *
 * -    :type     :search :
 * 2    :sname    :text   :id     :data   :
 * 3    :sep      :text   :
 * 4    :var      :text   :
 * 5    :output   :text   :radio  :data   :
 *
 * -    :type     :other  :
 * 6    :name     :text   :
 * 7    :file     :text   :
 *
 * -    :type     :normal :
 * 8    :timeout  :data   :dur    :timedata:
 * 9    :cmdc     :counts :
 * -    :cmd      :text   :id     :data   :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * -    :type     :script :
 * 10+  :name     :text   :
 * 11+  :var      :text   :
 * 12+  :cmdc     :counts :
 * -    :cmd      :text   :id     :data   :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * * data structure <order> type=temp(saved all current data)(except info)(2017/9/18 revised)
 * \    :        0:      1:      2:                3:
 * 0    :type     :temp   :only   :currentseldata   :
 * 1    :istack   :data   ://inner stack
 *
 * 2    :type     :search :only   :yes/no           :
 * 3    :sname    :text   :id     :data             :
 * 4    :sep      :text   :
 * 5    :var      :text   :
 * 6    :output   :text   :radio  :data             :
 *
 * 7    :type     :other  :only   :yes/no           :
 * 8    :name     :text   :
 * 9    :file     :text   :
 *
 * 10   :type     :normal :only   :yes/no           :
 * 11   :timeout  :data   :dur    :timedata         :
 * 12   :cmdc     :counts :
 * -    :cmd      :text   :id     :data             :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * 13   :type     :script :only   :yes/no           :
 * 14+  :name     :text   :
 * 15+  :var      :text   :
 * 16+  :cmdc     :counts :
 * -    :cmd      :text   :id     :data             :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * stacked structure
 * id
 * 0:info(fixed)
 * 1:local(fixed)
 * 2:process(dynamic)(normal, search, script, other)
 */

// tag define (index 0)
// all
#define TYPE                    "type"
#define CMDVALUE                "cmd"

// info
#define NAME_INFO               "name"
#define VERSION                 "type"
#define AUTHOR                  "author"
#define DESCRIPTION             "desc"
#define FILEINPUT               "finput"
#define FILEINPUT_SEARCHCHECK   "sinput"
#define FILESEARCH_NAME         "fsname"
#define RECURSIVE_ROOP          "rloop"
#define RECURSIVE_ROOPARGCOUNT  "rlarg"
#define RECURSIVE_ROOPCOUNT     "reloop"

// search
#define NAME_SEARCH             "sname"
#define SEARCH_SEPARATOR        "sep"
#define SEARCH_VARIANT          "var"
#define SEARCH_OUTPUTFILE       "output"

// other
#define NAME_PROFILE            "name"
#define PROFILE_FILEPATH        "file"

// normal
#define EXEC_TIMEOUT            "timeout"
#define EXEC_CMDARGCOUNT        "cmdc"

// extrafunc
#define NAME_PLUGIN             "name"
#define PLUGIN_FILEPATH         "file"
#define PLUGIN_CMDARGCOUNT      "cmdc"

// temp
#define STACKEDWIDGET_POSITION  "istack"

// local
#define ATTR_LOCALVARIANT       "lvar"

// value define (index 1)
#define TYPE_INFO               "info"
#define TYPE_EXEC               "normal"
#define TYPE_SEARCH             "search"
#define TYPE_SCRIPT             "script"
#define TYPE_ANOTHER            "other"

#define TYPE_ALLINCLUDE         "temp"
#define TYPE_LOCAL              "local"

// attribute define (index 2)
// all
#define ATTR_ONLY_SCHEDULER     "only"
#define ATTR_POSNUM             "id"
#define ATTR_RADIOBUTTONPOS     "radio"
#define ATTR_NONE               ""

// normal
#define ATTR_TIMEOUTMS          "dur"

// local
#define ATTR_LOCALVARCOUNT      "localc"
#define ATTR_LOCALVALUE         "lval"


class PROCESSXMLBUILDERSHARED_EXPORT ProcessXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
   explicit ProcessXmlBuilder(QObject *parent = 0);
   ~ProcessXmlBuilder();

   void setLoadPath(QString filepath);
   void setLoadBlankPath(QString filepath);
//   void delLoadPath();

   bool readItem(int itemid, QList<QStringList> *itemlist);
   bool readAllItem(QList<QList<QStringList> *> *itemlist);
   bool addItem(const QList<QStringList> *itemlist);
   bool deleteItem(int itemid);
   bool editItem(int itemid, const QList<QStringList> *itemlist);
   void swapItem(int beforeitemid, int afteritemid);
   void copyItem(int itemid);
   bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
   int count();
   QString fetch(QString tag, QString attr, const QList<QStringList> *loadbase);

private:
    void setSearchItemData(QString element, QList<QStringList> *list);
    void createDocument();
    const QString ROOTELEMENT = "profile";
    const QString FIRSTLAYER = "order";
    const QString ATTR = "id";

};

#endif // PROCESSXMLBUILDER_H
