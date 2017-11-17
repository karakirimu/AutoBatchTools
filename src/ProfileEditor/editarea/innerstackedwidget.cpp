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

void InnerStackedWidget::setEditOperator(EditOperator *op)
{
    editop = op;

    stackwidget = this->widget(0);
    name = stackwidget->findChild<QLineEdit *>("nameLineEdit");
    ver = stackwidget->findChild<QLineEdit *>("verLineEdit");
    author = stackwidget->findChild<QLineEdit *>("authorLineEdit");
    desc = stackwidget->findChild<QPlainTextEdit *>("descTextEdit");
    rlabel = stackwidget->findChild<QLabel *>("refreshDateLabel");

    connect(editop, &EditOperator::ui_selectindexUpdate, this, &InnerStackedWidget::setInfoDataList);
    connect(editop, &EditOperator::ui_selectindexUpdate, this, &InnerStackedWidget::moveStacked);
//    connect(editop, &EditOperator::editUpdate, this, &InnerStackedWidget::updateInfoDataList);

    connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);
}

void InnerStackedWidget::moveStacked(int index, int sendfrom)
{
    Q_UNUSED(sendfrom);
    if(index > 0){
        setCurrentIndex(1);
    }else{
        setCurrentIndex(0);
    }
}

//depends xml
void InnerStackedWidget::setInfoDataList(int index, int sendfrom)
{
    Q_UNUSED(sendfrom);

    if(index != 0) return;

    QList<QStringList> *list = new QList<QStringList>();

    if(editop->read(index, list)){

        //discon
        disconnect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        disconnect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        disconnect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        disconnect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);

        name->setText(list->at(1).at(1));
        ver->setText(list->at(2).at(1));
        author->setText(list->at(3).at(1));
        desc->setPlainText(list->at(4).at(1));
        rlabel->setText(list->at(5).at(1));

        //recon
        connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
        connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);
    }

    delete list;
}

//void InnerStackedWidget::updateInfoDataList(int index)
//{
//    if(index != 0) return;

//    QList<QStringList> *list = new QList<QStringList>();

//    if(editop->read(index, list)){

//        //discon
//        disconnect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);

//        name->setText(list->at(1).at(1));
//        ver->setText(list->at(2).at(1));
//        author->setText(list->at(3).at(1));
//        desc->setPlainText(list->at(4).at(1));
//        rlabel->setText(list->at(5).at(1));

//        //recon
//        connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);
//    }

//    delete list;
//}

//void InnerStackedWidget::clearInfoDataListForm()
//{
//    QList<QLineEdit *> menber = currentWidget()->findChildren<QLineEdit *>();
//    foreach(QLineEdit *edit, menber){
//        edit->clear();
//    }

//    QLabel *rlabel = currentWidget()->findChild<QLabel *>("refreshDateLabel");
//    rlabel->setText("-");
//}

void InnerStackedWidget::editTextAction(QString text)
{
    QString objname = this->sender()->objectName();
    qDebug() << "InnerStackedWidget::edittextaction : " << objname;
    editop->editTextAction(0, text, objname);
}

void InnerStackedWidget::editPlainTextAction()
{
    QString objname = this->sender()->objectName();
    qDebug() << "InnerStackedWidget::editplaintextaction : " << objname;
    editop->editTextAction(0, desc->toPlainText(), objname);
}

//void InnerStackedWidget::getInfoDataList(QStringList *list)
//{
//    if(currentIndex() == 1) return;
//    QWidget *widget = currentWidget();
//    QLineEdit *name = widget->findChild<QLineEdit *>("nameLineEdit");
//    QLineEdit *ver = widget->findChild<QLineEdit *>("verLineEdit");
//    QLineEdit *author = widget->findChild<QLineEdit *>("authorLineEdit");
//    QPlainTextEdit *desc = widget->findChild<QPlainTextEdit *>("descTextEdit");

//    list->append(name->text());
//    list->append(ver->text());
//    list->append(author->text());
//    list->append(desc->toPlainText());
//}
