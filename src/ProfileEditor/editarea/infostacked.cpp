/*
 * Copyright 2016-2020 karakirimu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "infostacked.h"

InfoStacked::InfoStacked(QWidget *parent) : QStackedWidget(parent)
{
}

InfoStacked::~InfoStacked()
{
}

void InfoStacked::setEditOperator(EditOperator *op)
{
    editop = op;

    stackwidget = this->widget(0);
    name = stackwidget->findChild<QLineEdit *>("nameLineEdit");
    ver = stackwidget->findChild<QLineEdit *>("verLineEdit");
    author = stackwidget->findChild<QLineEdit *>("authorLineEdit");
    desc = stackwidget->findChild<QLineEdit *>("descLineEdit");

    addbutton = stackwidget->findChild<QToolButton *>("addInputToolButton");
    editbutton = stackwidget->findChild<QToolButton *>("editInputToolButton");
    deletebutton = stackwidget->findChild<QToolButton *>("removeInputToolButton");

    finput = stackwidget->findChild<QCheckBox *>("allowInputCheckBox");
    sinput = stackwidget->findChild<QCheckBox *>("searchInputCheckBox");
    fscombo = stackwidget->findChild<SearchComboBox *>("searchInputComboBox");
    rloop = stackwidget->findChild<QCheckBox *>("loopCountInfCheckBox");

    rloopmax = stackwidget->findChild<QLineEdit *>("loopMaxLineEdit");
    rlargs = stackwidget->findChild<QLineEdit *>("loopArgumentsLineEdit");
    reloop = stackwidget->findChild<QLineEdit *>("loopRecursiveLineEdit");

    rloopmax->setValidator(new QIntValidator(0, 100000000, this));
    rlargs->setValidator(new QIntValidator(0, 100000000, this));
    reloop->setValidator(new QIntValidator(0, 100000000, this));


    rlabel = stackwidget->findChild<QLabel *>("loopMaxLabel");

    connect(editop, &EditOperator::processIndexUpdate, this, &InfoStacked::setInfoDataList);

    connect(name, &QLineEdit::textEdited, this, &InfoStacked::editProjectNameAction);
    connect(ver, &QLineEdit::textEdited, this, &InfoStacked::editVerAction);
    connect(author, &QLineEdit::textEdited, this, &InfoStacked::editAuthorAction);
    connect(desc, &QLineEdit::textEdited, this, &InfoStacked::editDescriptionAction);

    //connect action in search widget
    connect(addbutton, &QToolButton::clicked, fscombo, &SearchComboBox::addAction);
    connect(editbutton, &QToolButton::clicked, fscombo, &SearchComboBox::editAction);
    connect(deletebutton, &QToolButton::clicked, fscombo, &SearchComboBox::deleteAction);

    connect(finput, &QCheckBox::clicked, this, &InfoStacked::editCheckAction);
    connect(sinput, &QCheckBox::clicked, this, &InfoStacked::editCheckAction);
    connect(fscombo, &SearchComboBox::currentTextChanged, this, &InfoStacked::editInitialSearch);

    connect(rloop, &QCheckBox::clicked, this, &InfoStacked::editCheckAction);

    connect(rloopmax, &QLineEdit::textEdited, this, &InfoStacked::editTextValueAction);
    connect(rlargs, &QLineEdit::textEdited, this, &InfoStacked::editTextValueAction);
    connect(reloop, &QLineEdit::textEdited, this, &InfoStacked::editTextValueAction);

}

/**
 * @brief InfoStacked::switchStackedWidget
 * @param after
 * @param function
 * @return
 */
bool InfoStacked::switchStackedWidget(int after, int function)
{
    if((after == 2 && function == EditOperator::DELETE)
            || after == 0){

        setCurrentIndex(0);
        return true;

    }else{
        setCurrentIndex(1);
        return false;
    }
}

void InfoStacked::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");
    int command = sep.last().toInt();

    switch (command) {
    case UiCommandMap::UNDOREDO_EDIT:
        if(sep.at(0) == "0") setInfoDataList(static_cast<QString>(sep.at(0)).toInt(), -1, \
                            EditOperator::SELECT);
        break;
    case CommandMap::UNDOREDO_DELETE:
        if(sep.at(0) == "2") switchStackedWidget(0, EditOperator::SELECT);
        break;
    case CommandMap::UNDOREDO_ADD:
        if(sep.at(0) == "2") switchStackedWidget(1, EditOperator::SELECT);
        break;
    default:
        break;
    }
}

void InfoStacked::reloadAction()
{
    setInfoDataList(0, -1, EditOperator::SELECT);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void InfoStacked::setInfoDataList(int after, int unused1, int function)
{
    Q_UNUSED(unused1)


    // change stack and check loading is need or not.
    if(!switchStackedWidget(after, function)) return;

    // update information ui
    this->blockSignals(true);

    EditorCache list;

    if(!editop->read(after, &list)){
        return;
    }

    name->setText(list.info.name);
    ver->setText(list.info.version);
    author->setText(list.info.author);
    desc->setText(list.info.description);

    finput->setChecked(list.info.fileInput);
    sinput->setChecked(list.info.fileInputSearch);

    fscombo->reloadComboBoxItem();
    fscombo->setCurrentText(list.info.fileSearchName);
    rloop->setChecked(list.info.processAll);
    rloopmax->setText(QString::number(list.info.processMaxCount));
    rlargs->setText(QString::number(list.info.argumentsInOneLoop));
    reloop->setText(QString::number(list.info.recursiveLoopMax));

    bool check = list.info.fileInputSearch;
    addbutton->setVisible(check);
    editbutton->setVisible(check);
    deletebutton->setVisible(check);
    fscombo->setVisible(check);

    check = list.info.processAll;
    rlabel->setVisible(!check);
    rloopmax->setVisible(!check);

    this->blockSignals(false);
}

void InfoStacked::editAuthorAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editAuthorAction] : ";
#endif
    editop->textProjectAuthorAction(0,text);
}

void InfoStacked::editProjectNameAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editProjectNameAction] : ";
#endif
    editop->textProjectNameAction(0, text);
}

void InfoStacked::editVerAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editVerAction] : ";
#endif
    editop->textProjectVerAction(0, text);
}

void InfoStacked::editDescriptionAction(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editdescriptionaction] : " << this->sender()->objectName();
#endif
    editop->textProjectDescriptAction(0, text);
}

void InfoStacked::editInitialSearch(QString text)
{
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editInitialSearch] : ";
#endif

    editop->comboboxSearchAction(0,text, fscombo->currentIndex());
}

void InfoStacked::editCheckAction(bool check)
{
    QString objname = this->sender()->objectName();
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editcheckaction] : " << objname;
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

}

void InfoStacked::editTextValueAction(QString value)
{
    QString objname = this->sender()->objectName();
#ifdef QT_DEBUG
    qDebug() << "[InnerStackedWidget::editvalueaction] : " << objname;
#endif

    if(objname == "loopMaxLineEdit"){
        editop->spinLoopMaxAction(0,value.toInt());

    }else if(objname == "loopArgumentsLineEdit"){
        editop->spinLoopArgumentsAction(0,value.toInt());

    }else if(objname == "loopRecursiveLineEdit"){
        editop->spinLoopRecursiveAction(0,value.toInt());

    }
}
