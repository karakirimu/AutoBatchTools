#ifndef PROCESSXMLBUILDER_H
#define PROCESSXMLBUILDER_H

#include "processxmlbuilder_global.h"
#include <../xmlbuilder/xmlbuilder.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @class profile*_xml
 * @brief QList<QStringList> short description
 * @param data
 * @details
 * Left Listnum:
 * Top  xmlelementname(StringListnum):
 *
 * null     : no text
 * number   : usually used in integer number
 * text     : it means QString
 * others   : constant value
 *
 * data structure <order> type=info (2018/08/29 update)
 * \    :        0:      1:      2:      3:
 * 0    :type     :info   :only   :null   :
 * 1    :iname    :text   :
 * 2    :ver      :text   :
 * 3    :author   :text   :
 * 4    :desc     :text   :
 * 5    :finput   :yes/no :
 * 6    :sinput   :yes/no :
 * 7    :fsname   :text   :id     :number :
 * 8    :rloop    :yes/no :max    :number :
 * 9    :rlarg    :number :
 * 10   :reloop   :number :
 *
 *  data structure <order> type=local
 * \    :        0:      1:      2:      3:
 * 0    :type     :local  :
 * 1    :localc   :number :
 * 2    :lvar     :text   :lval   :text   :
 * -                    *
 * -                    *
 * -                    *
 * n         it follows to the end.
 *
 * data structure <order> type=normal
 * \    :        0:      1:      2:      3:
 * 0    :type     :normal :only   :data   :
 * 1    :nowait   :data   :
 * 2    :detach   :data   :
 * 3    :cmdc     :counts :
 * 4    :cmd      :text   :id     :data   :
 * 5                    *
 * 6                    *
 * 7                    *
 * 8         it follows to the end.
 *
 * data structure <order> type=search
 * \    :        0:      1:      2:      3:
 * 0    :type     :search :only   :data   :
 * 1    :sname    :text   :id     :data   :
 * 2    :sep      :text   :
 * 3    :var      :text   :
 * 4    :output   :text   :radio  :data   :
 * 5    :fitype   :data   :
 *
 * data structure <order> type=script
 * \    :        0:      1:      2:      3:
 * 0    :type     :script :only   :data   :
 * 1    :name     :text   :
 * 2    :var      :text   :
 * 3    :cmdc     :counts :
 * 4    :cmd      :text   :id     :data   :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * data structure <order> type=other(profile)
 * \    :        0:      1:      2:      3:
 * 0    :type     :other  :only   :data   :
 * 1    :prname   :text   :
 * 2    :prfile   :text   :
 *
 * * data structure <order> type=temp(saved all current data)(except info)(2018/8/29 revised)
 * \    :        0:      1:      2:            3:
 * 0    :type     :temp   :only   :yes/no   :
 * 1    :istack   :number :// position of specified attributes
 *
 * 2    :type     :search :only   :yes/no       :
 * 3    :sname    :text   :id     :number       :
 * 4    :sep      :text   :
 * 5    :var      :text   :
 * 6    :output   :text   :radio  :number       :
 *
 * 7    :type     :other  :only   :yes/no       :
 * 8    :prname   :text   :
 * 9    :prfile   :text   :
 *
 * 10   :type     :normal :only   :yes/no       :
 * 11   :timeout  :data   :dur    :number(time) :
 * 12   :cmdc     :counts :
 * -    :cmd      :text   :id     :number       :
 * -                    *
 * -                    *
 * -                    *
 * -         it follows to the end.
 *
 * 13   :type     :script :only   :yes/no   :
 * 14+  :plname   :text   :
 * 15+  :plfile   :text   :
 * 16+  :pcmdc    :number :
 * -    :cmd      :text   :id     :number   :
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

class PROCESSXMLBUILDERSHARED_EXPORT ProcessXmlBuilder : public Xmlbuilder
{
    Q_OBJECT
public:
   explicit ProcessXmlBuilder(QObject *parent = nullptr);
   ~ProcessXmlBuilder();

   void setLoadPath(QString filepath);
   void setLoadBlankPath(QString filepath);
//   void delLoadPath();

   bool readItem(int itemid, QList<QStringList> *itemlist);
   bool readAllItem(QList<QList<QStringList> *> *itemlist);
   bool writeAllItem(const QList<QList<QStringList> *> *itemlist);
   bool insertItem(const QList<QStringList> *itemlist, int index);
   bool deleteItem(int itemid);
   bool editItem(int itemid, const QList<QStringList> *itemlist);
   void swapItem(int beforeitemid, int afteritemid);
   bool overwriteItem(int itemid, const QList<QStringList> *itemlist);
   int count();

private:
    void setSearchItemData(QString element, QList<QStringList> *list);
    void createDocument();
    const QString ROOTELEMENT = "profile";
    const QString FIRSTLAYER = "order";
    const QString ATTR = "id";

};

#endif // PROCESSXMLBUILDER_H
