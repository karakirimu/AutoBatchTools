#ifndef INNERSTACKEDWIDGET_H
#define INNERSTACKEDWIDGET_H

#include "searchcombobox.h"

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QStackedWidget>
#include <QToolButton>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

#include <editoperator.h>

class InfoStacked : public QStackedWidget
{
public:
    explicit InfoStacked(QWidget *parent = nullptr);
    ~InfoStacked();

    void setEditOperator(EditOperator *op);

    void updateIndex(QString operation);
    void reloadAction();

public slots:
    void setInfoDataList(int after, int before, int function);

private slots:
    void editAuthorAction(QString text);
    void editProjectNameAction(QString text);
    void editVerAction(QString text);

    void editDescriptionAction(QString text);

    void editInitialSearch(QString text);

    void editCheckAction(bool check);
    void editTextValueAction(QString value);

private:
    bool moveStacked(int after, int function);

    EditOperator *editop;

    QWidget *stackwidget;
    QLineEdit *name;
    QLineEdit *ver;
    QLineEdit *author;
    QLineEdit *desc;

    QToolButton *addbutton;
    QToolButton *editbutton;
    QToolButton *deletebutton;

    QCheckBox *finput;
    QCheckBox *sinput;
    SearchComboBox *fscombo;
    QCheckBox *rloop;
    QLineEdit *rloopmax;
    QLineEdit *rlargs;
    QLineEdit *reloop;
    QLabel *rlabel;
};

#endif // INNERSTACKEDWIDGET_H
