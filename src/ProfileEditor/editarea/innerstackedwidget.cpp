#include "innerstackedwidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

InnerStackedWidget::InnerStackedWidget(QWidget *parent) : QStackedWidget(parent)
{
}

InnerStackedWidget::~InnerStackedWidget()
{
}

void InnerStackedWidget::moveStacked(int type)
{
    if(type == 0){
        setCurrentIndex(0);
    }else{
        setCurrentIndex(1);
    }
}

//depends xml
void InnerStackedWidget::setInfoDataList(QList<QStringList> *list, int firstpos)
{
    if(currentIndex() == 1) return;
    QWidget *widget = currentWidget();
    QLineEdit *name = widget->findChild<QLineEdit *>("nameLineEdit");
    QLineEdit *ver = widget->findChild<QLineEdit *>("verLineEdit");
    QLineEdit *author = widget->findChild<QLineEdit *>("authorLineEdit");
    QPlainTextEdit *desc = widget->findChild<QPlainTextEdit *>("descTextEdit");
    QLabel *rlabel = currentWidget()->findChild<QLabel *>("refreshDateLabel");

    name->setText(list->at(firstpos).at(1));
    ver->setText(list->at(firstpos + 1).at(1));
    author->setText(list->at(firstpos + 2).at(1));
    desc->setPlainText(list->at(firstpos + 3).at(1));
    rlabel->setText(list->at(firstpos + 4).at(1));;

}

void InnerStackedWidget::clearInfoDataListForm()
{
    QList<QLineEdit *> menber = currentWidget()->findChildren<QLineEdit *>();
    foreach(QLineEdit *edit, menber){
        edit->setText("");
    }

    QLabel *rlabel = currentWidget()->findChild<QLabel *>("refreshDateLabel");
    rlabel->setText("-");
}

void InnerStackedWidget::getInfoDataList(QStringList *list)
{
    if(currentIndex() == 1) return;
    QWidget *widget = currentWidget();
    QLineEdit *name = widget->findChild<QLineEdit *>("nameLineEdit");
    QLineEdit *ver = widget->findChild<QLineEdit *>("verLineEdit");
    QLineEdit *author = widget->findChild<QLineEdit *>("authorLineEdit");
    QPlainTextEdit *desc = widget->findChild<QPlainTextEdit *>("descTextEdit");

    list->append(name->text());
    list->append(ver->text());
    list->append(author->text());
    list->append(desc->toPlainText());
}
