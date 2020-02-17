#ifndef EDITORTAB_H
#define EDITORTAB_H

#include "commandtable.h"
#include "pluginscombobox.h"
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
    explicit EditorTab(QWidget *parent = nullptr);
    ~EditorTab();

    // ui <-> data communication functions
    void setConnection();
    void setEditOperator(EditOperator *op);

    //undo redo functions
    void updateIndex(QString operation);

private slots:
    //DEPENDS_XML data -> ui functions
    void setNormalDataList(QList<QStringList> *list);
    void setSearchDataList(QList<QStringList> *list);
    void setPluginDataList(QList<QStringList> *list);
    void setOtherDataList(QList<QStringList> *list);
    void setCombinedDataList(int after, int before, int function, int sendfrom);

    // DEPENDS_XML undo redo functions
    QString getTableData(int targetrow, int tablerow, QString loadtype);

    // DEPENDS_XML ui -> data functions
    void tabChanged(int index);

    void editCheckAction(bool check);
    void editRadioAction(bool);
//    void editValueAction(QString value);
    void editTextAction(QString text);
    void editTableAction(int index, QString str, int function);
    void editSwapTableAction(int indexbefore, int indexafter);
    void editDragDropTableAction(QList<int> indexbefore, int indexafter);

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
    QLineEdit *timeoutLineEdit;
    QCheckBox *runDetachCheckBox;
    QCheckBox *autoonly;
    CommandTable *ctablenormal;

    //search
    QWidget *widgetsearch;
    QToolButton *addbutton;
    QToolButton *editbutton;
    QToolButton *deletebutton;
    SearchComboBox *searchcombobox;
    QToolButton *openToolButton;
    QLineEdit *separatorLineEdit;
    VariantComboBox *localVariantComboBox;
    QLineEdit *outputLineEdit;
    QCheckBox *autoonly_2;
    QRadioButton *vari;
    QRadioButton *file;
    QRadioButton *fileOverWrite;
    QRadioButton *fileAppend;

    //plugins
    QWidget *widgetextra;
    PluginsComboBox *extrafunccombobox;
    QCheckBox *autoonly_3;
    CommandTable *ctableplugins;
    QToolButton *addbutton_e;
    QToolButton *deletebutton_e;
    QToolButton *pluginsetting;

    //other
    QWidget *otherwidget;
    ProfileComboBox *profilecombobox;
    QToolButton *addbutton_o;
    QToolButton *deletebutton_o;
    QCheckBox *autoonly_4;

};

#endif // EDITORTAB_H
