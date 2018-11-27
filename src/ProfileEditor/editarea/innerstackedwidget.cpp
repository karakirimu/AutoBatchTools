#include "innerstackedwidget.h"

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

    connect(editop, &EditOperator::ui_funcindexUpdate, this, &InnerStackedWidget::setInfoDataList);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &InnerStackedWidget::moveStacked);

    connect(name, &QLineEdit::textChanged, this, &InnerStackedWidget::editProjectNameAction);
    connect(ver, &QLineEdit::textChanged, this, &InnerStackedWidget::editVerAction);
    connect(author, &QLineEdit::textChanged, this, &InnerStackedWidget::editAuthorAction);
    connect(desc, &QPlainTextEdit::textChanged, this, &InnerStackedWidget::editDescriptionAction);

    //connect action in search widget
    connect(addbutton, &QToolButton::clicked, fscombo, &SearchComboBox::addAction);
    connect(editbutton, &QToolButton::clicked, fscombo, &SearchComboBox::editAction);
    connect(deletebutton, &QToolButton::clicked, fscombo, &SearchComboBox::deleteAction);

    connect(finput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(sinput, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(fscombo, &SearchComboBox::currentTextChanged, this, &InnerStackedWidget::editInitialSearch);
    connect(rloop, &QCheckBox::toggled, this, &InnerStackedWidget::editCheckAction);
    connect(rloopmax, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
    connect(rlargs, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);
    connect(reloop, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &InnerStackedWidget::editValueAction);

}

void InnerStackedWidget::moveStacked(int after, int before, int function, int sendfrom)
{
    Q_UNUSED(before); Q_UNUSED(function); Q_UNUSED(sendfrom);
    if(after > 0){
        setCurrentIndex(1);
    }else{
        setCurrentIndex(0);
    }
}

void InnerStackedWidget::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");

    if(sep.count() < 2){
        //edit
        setInfoDataList(static_cast<QString>(sep.at(0)).toInt(), -1, \
                            EditOperator::SELECT, EditOperator::MAINEDITOR);
    }else if(sep.at(0) == "1" && sep.at(1) == UNDOREDO_DELETE){
        //change stack
        moveStacked(0, -1, EditOperator::SELECT, EditOperator::MAINEDITOR);

    }else if(sep.at(0) == "1" && sep.at(1) == UNDOREDO_ADD){
        //change stack
        moveStacked(1, -1, EditOperator::SELECT, EditOperator::MAINEDITOR);

    }
}

void InnerStackedWidget::reloadAction()
{
    setInfoDataList(0, -1, EditOperator::SELECT, EditOperator::MAINEDITOR);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void InnerStackedWidget::setInfoDataList(int after, int before, int function, int sendfrom)
{
    Q_UNUSED(before); Q_UNUSED(sendfrom);

    // If the erased element is 1 and there are no subsequent elements.
    if(editop->getCacheSize() < 4
            && function == EditOperator::DELETE){
        //change stack
        moveStacked(0, -1, EditOperator::SELECT, EditOperator::MAINEDITOR);
    }

    // update information ui
    if(after != 0) return;

    QList<QStringList> *list = new QList<QStringList>();

    if(editop->read(after, list)){
        ProcessXmlListGenerator pxlg;

        this->blockSignals(true);

//        name->setText(list->at(1).at(1));
//        ver->setText(list->at(2).at(1));
//        author->setText(list->at(3).at(1));
//        desc->setPlainText(list->at(4).at(1));
        name->setText(pxlg.fetch(I_NAME, ATTR_NONE, list));
        ver->setText(pxlg.fetch(I_VERSION, ATTR_NONE, list));
        author->setText(pxlg.fetch(I_AUTHOR, ATTR_NONE, list));
        desc->setPlainText(pxlg.fetch(I_DESCRIPTION, ATTR_NONE, list));

        // versionup step
//        if(list->count() > 5){
//            finput->setChecked(VariantConverter::stringToBool(list->at(5).at(1)));
//            sinput->setChecked(VariantConverter::stringToBool(list->at(6).at(1)));
//            fscombo->reloadComboBoxItem();
//            fscombo->setCurrentText(list->at(7).at(1));
//            rloop->setChecked(VariantConverter::stringToBool(list->at(8).at(1)));
//            rloopmax->setValue(static_cast<QString>(list->at(8).at(3)).toInt());
//            rlargs->setValue(static_cast<QString>(list->at(9).at(1)).toInt());
//            reloop->setValue(static_cast<QString>(list->at(10).at(1)).toInt());

//            bool check = VariantConverter::stringToBool(list->at(6).at(1));
//            addbutton->setVisible(check);
//            editbutton->setVisible(check);
//            deletebutton->setVisible(check);
//            fscombo->setVisible(check);

//            check = VariantConverter::stringToBool(list->at(8).at(1));
//            rlabel->setVisible(!check);
//            rloopmax->setVisible(!check);

//        }

        finput->setChecked(VariantConverter::stringToBool(pxlg.fetch(I_FILEINPUT, ATTR_NONE, list)));
        sinput->setChecked(VariantConverter::stringToBool(pxlg.fetch(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, list)));
        fscombo->reloadComboBoxItem();
        fscombo->setCurrentText(pxlg.fetch(I_FILESEARCH_NAME, ATTR_NONE, list));
        rloop->setChecked(VariantConverter::stringToBool(pxlg.fetch(I_RECURSIVE_LOOP, ATTR_NONE, list)));
        rloopmax->setValue(static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOP, ATTR_MAXCOUNT, list)).toInt());
        rlargs->setValue(static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, list)).toInt());
        reloop->setValue(static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, list)).toInt());

        bool check = VariantConverter::stringToBool(pxlg.fetch(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, list));
        addbutton->setVisible(check);
        editbutton->setVisible(check);
        deletebutton->setVisible(check);
        fscombo->setVisible(check);

        check = VariantConverter::stringToBool(pxlg.fetch(I_RECURSIVE_LOOP, ATTR_NONE, list));
        rlabel->setVisible(!check);
        rloopmax->setVisible(!check);

        this->blockSignals(false);
    }

    delete list;
}

void InnerStackedWidget::editAuthorAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editAuthorAction : ";
#endif

    // textChanged will also be called if the text changes to empty.
    if(text == "") return;
    editop->textProjectAuthorAction(0,text);
}

void InnerStackedWidget::editProjectNameAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editProjectNameAction : ";
#endif

    // textChanged will also be called if the text changes to empty.
    if(text == "") return;
    editop->textProjectNameAction(0, text);
}

void InnerStackedWidget::editVerAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editVerAction : ";
#endif

    // textChanged will also be called if the text changes to empty.
    if(text == "") return;
    editop->textProjectVerAction(0, text);
}

void InnerStackedWidget::editDescriptionAction()
{
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editdescriptionaction : " << this->sender()->objectName();
#endif

    // textChanged will also be called if the text changes to empty.
    QString text = desc->toPlainText();
    if(text == "") return;
    editop->textProjectDescriptAction(0, text);
}

void InnerStackedWidget::editInitialSearch(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "InnerStackedWidget::editInitialSearch : ";
#endif

    editop->comboboxSearchAction(0,text, fscombo->currentIndex());
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

    //Compare it with the previous value and execute if there is a change.
    if(objname == "loopMaxSpinBox" && rloopmax->value() != value){
        editop->spinLoopMaxAction(0,value);
    }else if(objname == "loopArgumentsSpinBox" && rlargs->value() != value){
        editop->spinLoopArgumentsAction(0,value);
    }else if(objname == "loopRecursiveSpinBox" && reloop->value() != value){
        editop->spinLoopRecursiveAction(0,value);
    }
//    editop->editValueAction(0, value, objname);
}
