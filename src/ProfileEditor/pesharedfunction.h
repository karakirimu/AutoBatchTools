#ifndef PESHAREDFUNCTION_H
#define PESHAREDFUNCTION_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QFileDialog>
#include <QDebug>
#include <../processxmlbuilder/processxmlbuilder.h>
#include <../variantconverter/variantconverter.h>
#include <../convertchecker/convertchecker.h>

/**
 * @brief The PESharedFunction class
 * This class can create default list (to xml) and operate xml states.
 * basic Process r/w class
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
class PESharedFunction : public QObject
{
    Q_OBJECT
public:
    explicit PESharedFunction(QObject *parent = 0);
    ~PESharedFunction();

    //file io function
    void generateNewFile();
    void loadFile(QString filename);
    bool readItem(int itemid, QList<QStringList> *itemlist);

    //operating function
    void addItem();
    void deleteItem(int current);
    void editItem(int itemid, QList<QStringList> *itemlist);
    void copyItem(int current);
    void upItem(int current);
    void downItem(int current);

    QString openEditAction(QWidget *parent);
    QString saveEditAction(QWidget *parent);
    QString saveEditOverWriteAction(QWidget *parent);
    QString exportData(QWidget *parent);

    //check function
    int itemCount();

    //list create function
    void createInfoList(QList<QStringList> *newlist, QStringList *list);
    void createLocalList(QList<QStringList> *newlist, QStringList *list);
    void createNormalList(QList<QStringList> *newlist, QStringList *list);
    void createSearchList(QList<QStringList> *newlist, QStringList *list);
    void createScriptList(QList<QStringList> *newlist, QStringList *list);
    void createOtherList(QList<QStringList> *newlist, QStringList *list);

    //change "Temp" structure to loadable xml structure
    void convertToDefaultList(QList<QStringList> *temp);

    //check file is edited.
    int checkFileEdited();

    //check info
    int setInformationType(QString type);

    //derived from xml
    int firstPosNormal() const;
    int firstPosSearch() const;
    int firstPosScript() const;
    int firstPosOther() const;

signals:
    void fileNameChanged(QString, QString);

public slots:

private:
    QString saveAction(QString fileName);

    //tmp filename
    QString tmpbku;
    QString tmpfilepath;

    //current loaded filename
    QString loadedfile;

    //xml builder
    ProcessXmlBuilder *builder;
};

#endif // PESHAREDFUNCTION_H
