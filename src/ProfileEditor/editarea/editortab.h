#ifndef EDITORTAB_H
#define EDITORTAB_H

#include "commandtable.h"
#include "extrafunctionscombobox.h"
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
#include <pesharedfunction.h>
#include <../variantconverter/variantconverter.h>

class EditorTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorTab(QWidget *parent = Q_NULLPTR);
    ~EditorTab();

    void setConnection();

    enum{NORMAL, SEARCH, EXTRAFUNC, OTHER};

public slots:
    //function depends function
    void moveTabFromXml(int num);

    //xml depends functions DEPENDS_XML
    void setNormalDataList(QList<QStringList> *list, int firstpos);
    void setSearchDataList(QList<QStringList> *list, int firstpos);
    void setScriptDataList(QList<QStringList> *list, int firstpos);
    void setOtherDataList(QList<QStringList> *list, int firstpos);
    void setTempDataList(QList<QStringList> *list, PESharedFunction *function);

    void getNormalDataList(QStringList *list);
    void getSearchDataList(QStringList *list);
    void getScriptDataList(QStringList *list);
    void getOtherDataList(QStringList *list);

    bool getCurrentIndexOnlyChecked();

private:
    void openSavefile();
};

#endif // EDITORTAB_H
