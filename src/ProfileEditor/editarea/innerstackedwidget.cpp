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

    addbutton = stackwidget->findChild<QToolButton *>("addInputToolButton");
    editbutton = stackwidget->findChild<QToolButton *>("editInputToolButton");
    deletebutton = stackwidget->findChild<QToolButton *>("removeInputToolButton");

    finput = stackwidget->findChild<QCheckBox *>("allowInputCheckBox");
    sinput = stackwidget->findChild<QCheckBox *>("searchInputCheckBox");
    fscombo = stackwidget->findChild<SearchComboBox *>("searchInputComboBox");
    rloop = stackwidget->findChild<QCheckBox *>("loopCountInfCheckBox");
    rloopmax = stackwidget->findChild<QSpinBox *>("loopMaxSpinBox");
    rlargs = stackwidget->findChild<QSpinBox *>("loopArgumentsSpinBox");
    reloop = stackwidget->findChild<QSpinBox *>("loopRecursiveSpinBox");
    rlabel = stackwidget->findChild<QLabel *>("loopMaxLabel");
//    searchinput = stackwidget->findChild<QFrame *>("searchInput");
//    loopcountmax = stackwidget->findChild<QFrame *>("loopCountMax");

    connect(editop, &EditOperator::ui_selectindexUpdate, this, &InnerStackedWidget::setInfoDataList);
    connect(editop, &EditOperator::ui_selectindexUpdate, this, &InnerStackedWidget::moveStacked);
//    connect(editop, &EditOperator::editUpdate, this, &InnerStackedWidget::updateInfoDataList);

    connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
    connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);

    //connect action in search widget
    connect(addbutton, &QToolButton::clicked, fscombo, &SearchComboBox::addAction);
    connect(editbutton, &QToolButton::clicked, fscombo, &SearchComboBox::editAction);
    connect(deletebutton, &QToolButton::clicked, fscombo, &SearchComboBox::deleteAction);

    connect(finput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(sinput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(fscombo, &SearchComboBox::currentTextChanged, this, &InnerStackedWidget::editTextAction);
    connect(rloop, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(rloopmax, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
    connect(rlargs, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
    connect(reloop, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);

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

///DEPENDS_XML DEPENDS_UI PROCESS
void InnerStackedWidget::setInfoDataList(int index, int sendfrom)
{
    Q_UNUSED(sendfrom);

    if(index != 0) return;

    QList<QStringList> *list = new QList<QStringList>();

    if(editop->read(index, list)){

        //discon
//        disconnect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);
//        disconnect(finput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        disconnect(sinput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        disconnect(fscombo, &SearchComboBox::currentTextChanged, this, &InnerStackedWidget::editTextAction);
//        disconnect(rloop, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        disconnect(rloopmax, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
//        disconnect(rlargs, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
//        disconnect(reloop, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
        this->blockSignals(true);

        name->setText(list->at(1).at(1));
        ver->setText(list->at(2).at(1));
        author->setText(list->at(3).at(1));
        desc->setPlainText(list->at(4).at(1));

        // versionup step
        if(list->count() > 5){
            finput->setChecked(VariantConverter::stringToBool(list->at(5).at(1)));
            sinput->setChecked(VariantConverter::stringToBool(list->at(6).at(1)));
            fscombo->reloadComboBoxItem();
            fscombo->setCurrentText(list->at(7).at(1));
            rloop->setChecked(VariantConverter::stringToBool(list->at(8).at(1)));
            rloopmax->setValue(((QString)list->at(8).at(3)).toInt());
            rlargs->setValue(((QString)list->at(9).at(1)).toInt());
            reloop->setValue(((QString)list->at(10).at(1)).toInt());

            bool check = VariantConverter::stringToBool(list->at(6).at(1));
            addbutton->setVisible(check);
            editbutton->setVisible(check);
            deletebutton->setVisible(check);
            fscombo->setVisible(check);

            check = VariantConverter::stringToBool(list->at(8).at(1));
            rlabel->setVisible(!check);
            rloopmax->setVisible(!check);

        }

//        rlabel->setText(list->at(5).at(1));

        //recon
//        connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editTextAction);
//        connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editPlainTextAction);
//        connect(finput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        connect(sinput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        connect(fscombo, &SearchComboBox::currentTextChanged, this, &InnerStackedWidget::editTextAction);
//        connect(rloop, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
//        connect(rloopmax, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
//        connect(rlargs, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
//        connect(reloop, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
        this->blockSignals(false);
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
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::edittextaction : " << objname;
#endif
    if(objname == "searchInputComboBox"){
        editop->comboboxSearchAction(0, text, fscombo->currentIndex());
    }else{
        editop->editTextAction(0, text, objname);
    }
}

void InnerStackedWidget::editPlainTextAction()
{
    QString objname = this->sender()->objectName();
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editplaintextaction : " << objname;
#endif
    editop->editTextAction(0, desc->toPlainText(), objname);
}

void InnerStackedWidget::editCheckAction(bool check)
{
    QString objname = this->sender()->objectName();
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editcheckaction : " << objname;
#endif
    //allowInputCheckBox
    //loopCountInfCheckBox
    if(objname == "searchInputCheckBox"){
        fscombo->setVisible(check);
        addbutton->setVisible(check);
        editbutton->setVisible(check);
        deletebutton->setVisible(check);

        this->repaint();
        editop->checkSearchInputAction(0,check);

    }else if(objname == "loopCountInfCheckBox"){
        rlabel->setVisible(!check);
        rloopmax->setVisible(!check);

        editop->checkLoopInfAction(0, check);
    }else if(objname == "allowInputCheckBox"){
        editop->checkAllowInputAction(0,check);
    }

    //searchInputCheckBox
//    editop->editCheckAction(0,check,objname);
}

void InnerStackedWidget::editValueAction(int value)
{
    QString objname = this->sender()->objectName();
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editvalueaction : " << objname;
#endif

    if(objname == "loopMaxSpinBox"){
        editop->spinLoopMaxAction(0,value);
    }else if(objname == "loopArgumentsSpinBox"){
        editop->spinLoopArgumentsAction(0,value);
    }else if(objname == "loopRecursiveSpinBox"){
        editop->spinLoopRecursiveAction(0,value);
    }
//    editop->editValueAction(0, value, objname);
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
