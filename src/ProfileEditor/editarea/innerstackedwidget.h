#ifndef INNERSTACKEDWIDGET_H
#define INNERSTACKEDWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPlainTextEdit>
#include <QStackedWidget>
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
    void clearInfoDataListForm();
//    void getInfoDataList(QStringList *list);

private slots:
    void editTextAction(QString text);
    void editPlainTextAction();

private:

    EditOperator *editop;
    QWidget *stackwidget;
    QLineEdit *name;
    QLineEdit *ver;
    QLineEdit *author;
    QPlainTextEdit *desc;
    QLabel *rlabel;
};

#endif // INNERSTACKEDWIDGET_H
