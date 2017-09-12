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
 * 8    :nowait   :data   :
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
 */

class PROCESSXMLBUILDERSHARED_EXPORT ProcessXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
    PROCESSXMLBUILDERSHARED_EXPORT explicit ProcessXmlBuilder(QObject *parent = 0);
    PROCESSXMLBUILDERSHARED_EXPORT ~ProcessXmlBuilder();

    PROCESSXMLBUILDERSHARED_EXPORT void setLoadPath(QString filepath);
//    PROCESSXMLBUILDERSHARED_EXPORT void delLoadPath();

    PROCESSXMLBUILDERSHARED_EXPORT bool readItem(int itemid, QList<QStringList> *itemlist);
    PROCESSXMLBUILDERSHARED_EXPORT bool addItem(const QList<QStringList> *itemlist);
    PROCESSXMLBUILDERSHARED_EXPORT bool deleteItem(int itemid);
    PROCESSXMLBUILDERSHARED_EXPORT bool editItem(int itemid, const QList<QStringList> *itemlist);
    PROCESSXMLBUILDERSHARED_EXPORT void swapItem(int beforeitemid, int afteritemid);
    PROCESSXMLBUILDERSHARED_EXPORT void copyItem(int itemid);
    PROCESSXMLBUILDERSHARED_EXPORT bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
    PROCESSXMLBUILDERSHARED_EXPORT void createDocument();
    PROCESSXMLBUILDERSHARED_EXPORT int count();

    PROCESSXMLBUILDERSHARED_EXPORT int firstPosTempNormal() const;
    PROCESSXMLBUILDERSHARED_EXPORT int firstPosTempSearch() const;
    PROCESSXMLBUILDERSHARED_EXPORT int firstPosTempScript() const;
    PROCESSXMLBUILDERSHARED_EXPORT int firstPosTempOther() const;

private:
    void setSearchItemData(QString element, QList<QStringList> *list);
    const QString ROOTELEMENT = "profile";
    const QString FIRSTLAYER = "order";
    const QString ATTR = "id";

};

#endif // PROCESSXMLBUILDER_H
