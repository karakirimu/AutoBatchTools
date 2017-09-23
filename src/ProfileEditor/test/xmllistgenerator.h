#ifndef XMLLISTGENERATOR_H
#define XMLLISTGENERATOR_H

#include <QObject>
#include <QHash>

/**
 * @brief The XmlListGenerator class
 * This class can create default list (to xml) and operate xml states.
 */
/**
 * @brief create list type arguments of QStringList
 * QStringList structure createInfoList
 * 0    :name_text  :
 * 1    :ver_text   :
 * 2    :author_text:
 * 3    :desc_text  :
 *
 * QStringList structure createLocalList
 * 0    :localc_counts:
 * 1    :lvar_text    :
 * 2    :lval_text    :
 * -           *
 * -           *
 * -           *
 * - They follow to the end by turns.
 *
 * QStringList structure createNormalList
 * 0    :only_bool   :
 * 1    :timeout_bool:
 * 2    :timeout_ms  :
 * 3    :cmdc_counts :
 * 4    :cmd_text    :
 * -          *
 * -          *
 * -          *
 * - it follows to the end.
 *
 * QStringList structure createSearchList
 * 0    :only_bool   :
 * 1    :name_text   :
 * 2    :name_id_data:
 * 3    :sep_text    :
 * 4    :variant_text:
 * 5    :output_text :
 * 6    :radio_data  :
 *
 * QStringList structure createScriptList
 * 0    :only_bool  :
 * 1    :name_text  :
 * 2    :var_text   :
 * 3    :cmdc_counts:
 * 4    :cmd_text   :
 * -          *
 * -          *
 * -          *
 * - it follows to the end.
 *
 * QStringList structure createOtherList
 * 0    :only_bool   :
 * 1    :name_text   :
 * 2    :file_text   :
 *
 */
class XmlListGenerator : public QObject
{
    Q_OBJECT
public:
    explicit XmlListGenerator(QObject *parent = nullptr);
    ~XmlListGenerator();

    enum{NORMAL, SEARCH, EXTRAFUNC, OTHER};

    //list add structure
    void createNewList(QList<QStringList> *newlist);

    //list create function
    void createInfoList(QList<QStringList> *newlist, QStringList *list);
    void createLocalList(QList<QStringList> *newlist, QStringList *list);
    void createNormalList(QList<QStringList> *newlist, QStringList *list);
    void createSearchList(QList<QStringList> *newlist, QStringList *list);
    void createExtraFuncList(QList<QStringList> *newlist, QStringList *list);
    void createOtherList(QList<QStringList> *newlist, QStringList *list);

    //combined above list
    void createCombineList(QList<QStringList> *newlist, int index, QHash<int, QStringList> *combine);

    //change combined structure to separated structure
    void createSeparateList(QList<QStringList> *ctos);

    //check info
    int getType(QString type);

signals:

public slots:
};

#endif // XMLLISTGENERATOR_H
