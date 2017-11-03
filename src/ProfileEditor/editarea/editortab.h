#ifndef EDITORTAB_H
#define EDITORTAB_H

#include "commandtable.h"
#include "extrafunctionscombobox.h"
#include "profilecombobox.h"
#include "searchcombobox.h"
#include "variantcombobox.h"

#include <QTabWidget>
#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QToolButton>
#include <QTableWidget>
#include <QRadioButton>
#include <editoperator.h>
//#include <pesharedfunction.h>
#include <../variantconverter/variantconverter.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditorTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorTab(QWidget *parent = Q_NULLPTR);
    ~EditorTab();

    void setConnection();
    void setEditOperator(EditOperator *op);

//    enum{NORMAL, SEARCH, EXTRAFUNC, OTHER};

public slots:
    //function depends function
//    void moveTabFromXml(int num);

    //xml depends functions DEPENDS_XML
    void setNormalDataList(QList<QStringList> *list, int firstpos);
    void setSearchDataList(QList<QStringList> *list, int firstpos);
    void setExtraFuncDataList(QList<QStringList> *list, int firstpos);
    void setOtherDataList(QList<QStringList> *list, int firstpos);
    void setCombinedDataList(int index, int selectfrom);

//    void getNormalDataList(QStringList *list);
//    void getSearchDataList(QStringList *list);
//    void getExtraFuncDataList(QStringList *list);
//    void getOtherDataList(QStringList *list);

    bool getCurrentIndexOnlyChecked();

private slots:
    void tabChanged(int index);

    void editCheckAction(bool check);
    void editRadioAction(bool);
    void editValueAction(int value);
    void editTextAction(QString text);
    void editTableAction(int index, QString str, int function);
    void editSwapTableAction(int indexbefore, int indexafter);

private:
    void openSavefile();

    EditOperator *editop;
    ProcessXmlListGenerator xgen;
    //current xml id
    int currentid = -1;

    //set only flag (not edit flag)
//    bool edited = false;

    //ui objects

    //normal
    QWidget *widgetnormal;
    QCheckBox *timeoutCheckBox;
    QSpinBox *tospin;
    QCheckBox *autoonly;
    CommandTable *ctablenormal;

    //search
    QWidget *widgetsearch;
    QPushButton *addbutton;
    QPushButton *editbutton;
    QPushButton *deletebutton;
    SearchComboBox *searchcombobox;
    QToolButton *openButton;
    QLineEdit *separatorLineEdit;
    VariantComboBox *localVariantComboBox;
    QLineEdit *outputLineEdit;
    QCheckBox *autoonly_2;
    QRadioButton *vari;
    QRadioButton *file;

    //extrafunc
    QWidget *widgetextra;
    ExtraFunctionsComboBox *extrafunccombobox;
    QCheckBox *autoonly_3;
    CommandTable *extrafuncTableWidget;
    QToolButton *addbutton_e;
    QToolButton *deletebutton_e;
    QPushButton *pluginsetting;

    //other
    QWidget *otherwidget;
    ProfileComboBox *profilecombobox;
    QCheckBox *autoonly_4;

};

#endif // EDITORTAB_H
