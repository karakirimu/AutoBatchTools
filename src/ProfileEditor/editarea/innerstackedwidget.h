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
#include <editoperator.h>

class InnerStackedWidget : public QStackedWidget
{
public:
    explicit InnerStackedWidget(QWidget *parent = nullptr);
    ~InnerStackedWidget();

    void setEditOperator(EditOperator *op);
    void moveStacked(int index, int sendfrom);
public slots:
    void setInfoDataList(int index, int sendfrom);
//    void updateInfoDataList(int index);
//    void clearInfoDataListForm();
//    void getInfoDataList(QStringList *list);

private slots:
    void editTextAction(QString text);
    void editPlainTextAction();
    void editCheckAction(bool check);
    void editValueAction(int value);

private:

    EditOperator *editop;
    QWidget *stackwidget;
    QLineEdit *name;
    QLineEdit *ver;
    QLineEdit *author;
    QPlainTextEdit *desc;

    QToolButton *addbutton;
    QToolButton *editbutton;
    QToolButton *deletebutton;

    QCheckBox *finput;
    QCheckBox *sinput;
    SearchComboBox *fscombo;
    QCheckBox *rloop;
    QSpinBox *rloopmax;
    QSpinBox *rlargs;
    QSpinBox *reloop;
    QLabel *rlabel;
};

#endif // INNERSTACKEDWIDGET_H
