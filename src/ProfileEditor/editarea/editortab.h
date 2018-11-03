#ifndef EDITORTAB_H
#define EDITORTAB_H

#include "commandtable.h"
#include "extrafunctionscombobox.h"
#include "../profilecombobox/profilecombobox.h"
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
#include <QDesktopWidget>
//#include <pesharedfunction.h>
#include <../variantconverter/variantconverter.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../plugins/ExtraPluginInterface/extraplugininterface.h>

//ui object strings
#define SEARCH_ADD      "searchAddButton"
#define SEARCH_EDIT     "searchEditButton"
#define SEARCH_DELETE   "searchDeleteButton"
#define SEARCH_COMBO    "searchComboBox"

class EditorTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorTab(QWidget *parent = Q_NULLPTR);
    ~EditorTab();

    void setConnection();
    void setEditOperator(EditOperator *op);

private slots:
    //xml depends functions DEPENDS_XML
    void setNormalDataList(QList<QStringList> *list, int firstpos);
    void setSearchDataList(QList<QStringList> *list, int firstpos);
    void setExtraFuncDataList(QList<QStringList> *list, int firstpos);
    void setOtherDataList(QList<QStringList> *list, int firstpos);
    void setCombinedDataList(int after, int before, int function, int sendfrom);

//    bool getCurrentIndexOnlyChecked();

    void tabChanged(int index);

    void editCheckAction(bool check);
    void editRadioAction(bool);
    void editValueAction(int value);
    void editTextAction(QString text);
    void editTableAction(int index, QString str, int function);
    void editSwapTableAction(int indexbefore, int indexafter);

    void pluginSettingsClicked();

private:
    void openSavefile();

    EditOperator *editop;
    ProcessXmlListGenerator xgen;
    //current xml id
    int currentid = -1;

    //ui objects
    //normal
    QWidget *widgetnormal;
    QCheckBox *timeoutCheckBox;
    QSpinBox *tospin;
    QCheckBox *autoonly;
    CommandTable *ctablenormal;

    //search
    QWidget *widgetsearch;
    QToolButton *addbutton;
    QToolButton *editbutton;
    QToolButton *deletebutton;
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
    QToolButton *addbutton_o;
    QToolButton *deletebutton_o;
    QCheckBox *autoonly_4;

};

#endif // EDITORTAB_H
