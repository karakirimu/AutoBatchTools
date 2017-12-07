#include "editortab.h"

EditorTab::EditorTab(QWidget *parent)
    : QTabWidget(parent)
{

}

EditorTab::~EditorTab()
{

}


// Constructor can not initialize connection.
void EditorTab::setConnection()
{
    widgetsearch = this->widget(ProcessXmlListGenerator::SEARCH);
    addbutton = widgetsearch->findChild<QPushButton *>("searchAddButton");
    editbutton = widgetsearch->findChild<QPushButton *>("searchEditButton");
    deletebutton = widgetsearch->findChild<QPushButton *>("searchDeleteButton");
    searchcombobox = widgetsearch->findChild<SearchComboBox *>("searchComboBox");
    openButton = widgetsearch->findChild<QToolButton *>("openToolButton");

    widgetextra = this->widget(ProcessXmlListGenerator::EXTRAFUNC);
    extrafunccombobox = widgetextra->findChild<ExtraFunctionsComboBox *>("extrafuncComboBox");
    addbutton_e = widgetextra->findChild<QToolButton *>("extrafuncAddButton");
    deletebutton_e = widgetextra->findChild<QToolButton *>("extrafuncDeleteButton");
    pluginsetting = widgetextra->findChild<QPushButton *>("pluginSettingButton");

    otherwidget = this->widget(ProcessXmlListGenerator::OTHER);
    profilecombobox = otherwidget->findChild<ProfileComboBox *>("profileComboBox");
    addbutton_o = otherwidget->findChild<QToolButton *>("otherAddButton");
    deletebutton_o = otherwidget->findChild<QToolButton *>("otherDeleteButton");

    //connect action in search widget
    connect(addbutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::addAction);
    connect(editbutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::editAction);
    connect(deletebutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::deleteAction);
    connect(openButton, &QToolButton::clicked, this, &EditorTab::openSavefile); 

    //connect action in extrafunc widget
    connect(addbutton_e, &QAbstractButton::clicked, extrafunccombobox, &ExtraFunctionsComboBox::addItemAction);
    connect(deletebutton_e, &QAbstractButton::clicked, extrafunccombobox, &ExtraFunctionsComboBox::deleteAction);
    connect(extrafunccombobox, &ExtraFunctionsComboBox::pluginChanged, pluginsetting, &QPushButton::setEnabled);

    //connect action in profile widget
    connect(addbutton_o, &QToolButton::clicked, profilecombobox, &ProfileComboBox::addItemAction);
    connect(deletebutton_o, &QToolButton::clicked, profilecombobox, &ProfileComboBox::deleteItemAction);

    //this tab change
    connect(this, &EditorTab::currentChanged, this, &EditorTab::tabChanged);
}

void EditorTab::setEditOperator(EditOperator *op)
{
    editop = op;

    // load normal widget ui objects
    widgetnormal = this->widget(ProcessXmlListGenerator::NORMAL);
    timeoutCheckBox = widgetnormal->findChild<QCheckBox *>("timeoutCheckBox");
    tospin = widgetnormal->findChild<QSpinBox *>("timeoutSpinBox");
    autoonly = widgetnormal->findChild<QCheckBox *>("autoOnlyCheckBox");
    ctablenormal = widgetnormal->findChild<CommandTable *>("cmdTableWidget");

    // load search widget ui objects
    localVariantComboBox = widgetsearch->findChild<VariantComboBox *>("localVariantComboBox");
    outputLineEdit = widgetsearch->findChild<QLineEdit *>("outputLineEdit");
    separatorLineEdit = widgetsearch->findChild<QLineEdit *>("separatorLineEdit");
    autoonly_2 = widgetsearch->findChild<QCheckBox *>("autoOnlyCheckBox_2");
    vari = widgetsearch->findChild<QRadioButton *>("variRadioButton");
    file = widgetsearch->findChild<QRadioButton *>("fileRadioButton");

    //set editop
    localVariantComboBox->setEditOperator(op);

    // load extrafunc widget ui objects
    autoonly_3 = widgetextra->findChild<QCheckBox *>("autoOnlyCheckBox_3");
    extrafuncTableWidget = widgetextra->findChild<CommandTable *>("extrafuncTableWidget");

    // load other widget ui objects
    profilecombobox = otherwidget->findChild<ProfileComboBox *>("profileComboBox");
    autoonly_4 = otherwidget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    //index update
    connect(editop, &EditOperator::ui_selectindexUpdate, this, &EditorTab::setCombinedDataList);

    //index edit (table is ignored)
    connect(timeoutCheckBox, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    connect(tospin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            , this, &EditorTab::editValueAction);
    connect(autoonly, &QCheckBox::toggled, this, &EditorTab::editCheckAction);

    connect(searchcombobox, &SearchComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(separatorLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(localVariantComboBox, &VariantComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(outputLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(autoonly_2, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    connect(vari, &QRadioButton::toggled, this, &EditorTab::editRadioAction);
    connect(file, &QRadioButton::toggled, this, &EditorTab::editRadioAction);

    connect(extrafunccombobox, &ExtraFunctionsComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(autoonly_3, &QCheckBox::toggled, this, &EditorTab::editCheckAction);

    connect(profilecombobox, &ProfileComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(autoonly_4, &QCheckBox::toggled, this, &EditorTab::editCheckAction);

    //index edit (table only)
    connect(ctablenormal, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(ctablenormal, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
    connect(extrafuncTableWidget, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(extrafuncTableWidget, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
}

//DEPENDS_XML
void EditorTab::setNormalDataList(QList<QStringList> *list, int firstpos)
{
    disconnect(timeoutCheckBox, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    disconnect(tospin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            , this, &EditorTab::editValueAction);
    disconnect(autoonly, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    disconnect(ctablenormal, &CommandTable::updateTable, this, &EditorTab::editTableAction);

    int counter = VariantConverter::stringToInt(list->at(firstpos + 2).at(1));
    timeoutCheckBox->setChecked(VariantConverter::stringToBool(list->at(firstpos + 1).at(1)));
    tospin->setValue(((QString)list->at(firstpos + 1).at(3)).toInt());
    ctablenormal->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        ctablenormal->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 3 + i).at(1)));
    }

    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    connect(timeoutCheckBox, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    connect(tospin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged)
            , this, &EditorTab::editValueAction);
    connect(autoonly, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    connect(ctablenormal, &CommandTable::updateTable, this, &EditorTab::editTableAction);
}

//DEPENDS_XML
void EditorTab::setSearchDataList(QList<QStringList> *list, int firstpos)
{
    //reload combobox
    disconnect(searchcombobox, &SearchComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    disconnect(separatorLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    disconnect(localVariantComboBox, &VariantComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    disconnect(outputLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    disconnect(autoonly_2, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    disconnect(vari, &QRadioButton::toggled, this, &EditorTab::editRadioAction);
    disconnect(file, &QRadioButton::toggled, this, &EditorTab::editRadioAction);

    searchcombobox->reloadComboBoxItem();
    searchcombobox->setCurrentText(list->at(firstpos + 1).at(1));

    separatorLineEdit->setText(list->at(firstpos + 2).at(1));
    localVariantComboBox->insertItem(0,list->at(firstpos + 3).at(1));
    outputLineEdit->setText(list->at(firstpos + 4).at(1));

    if(list->at(firstpos + 4).at(3) == "0"){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    autoonly_2->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    connect(searchcombobox, &SearchComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(separatorLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(localVariantComboBox, &VariantComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(outputLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(autoonly_2, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    connect(vari, &QRadioButton::toggled, this, &EditorTab::editRadioAction);
    connect(file, &QRadioButton::toggled, this, &EditorTab::editRadioAction);

}

//DEPENDS_XML
void EditorTab::setExtraFuncDataList(QList<QStringList> *list, int firstpos)
{
    //reset combobox
    disconnect(extrafunccombobox, &ExtraFunctionsComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    extrafunccombobox->reloadComboBoxItem();
    extrafunccombobox->setCurrentText(list->at(firstpos + 1).at(1));
    connect(extrafunccombobox, &ExtraFunctionsComboBox::currentTextChanged, this, &EditorTab::editTextAction);

//    vcb->insertItem(0,list->at(firstpos + 1).at(1));
    int counter = VariantConverter::stringToInt(list->at(firstpos + 3).at(1));
    disconnect(extrafuncTableWidget, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    extrafuncTableWidget->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        extrafuncTableWidget->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 4 + i).at(1)));
    }
    connect(extrafuncTableWidget, &CommandTable::updateTable, this, &EditorTab::editTableAction);

    disconnect(autoonly_3, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    autoonly_3->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
    connect(autoonly_3, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
}

//DEPENDS_XML
void EditorTab::setOtherDataList(QList<QStringList> *list, int firstpos)
{
    disconnect(profilecombobox, &ProfileComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    profilecombobox->reloadComboBoxItem();
    profilecombobox->setCurrentText(list->at(firstpos + 1).at(1));
    connect(profilecombobox, &ProfileComboBox::currentTextChanged, this, &EditorTab::editTextAction);

    disconnect(autoonly_4, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
    autoonly_4->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));
    connect(autoonly_4, &QCheckBox::toggled, this, &EditorTab::editCheckAction);
}

void EditorTab::setCombinedDataList(int index, int selectfrom)
{
    Q_UNUSED(selectfrom)
    QList<QStringList> *list = new QList<QStringList>();
    QHash<int, int> hlist;

    //avoid multiple update
    if(editop->read(index, list) && currentid != index){

        //set current id
        qDebug() << ((QString)("EditorTab : currentid update to %1")).arg(index);
        currentid = index;
//        edited = false;
        xgen.getListStructure(list, &hlist);

        //no file showing module
    //    int cmdskip = VariantConverter::stringToInt(list->at(sfunction->firstPosNormal() + 1).at(1));

        //set widget selection
        if(hlist.count() > 1){
            disconnect(this, &EditorTab::currentChanged, this, &EditorTab::tabChanged);
            setCurrentIndex(VariantConverter::stringToInt(list->at(1).at(1)));
            connect(this, &EditorTab::currentChanged, this, &EditorTab::tabChanged);
        }

        //load type=normal
        if(hlist.contains(ProcessXmlListGenerator::NORMAL))
            setNormalDataList(list, hlist.value(ProcessXmlListGenerator::NORMAL));

        //load type=search
        if(hlist.contains(ProcessXmlListGenerator::SEARCH))
            setSearchDataList(list, hlist.value(ProcessXmlListGenerator::SEARCH));

        //load type=extrafunc
        if(hlist.contains(ProcessXmlListGenerator::EXTRAFUNC))
            setExtraFuncDataList(list, hlist.value(ProcessXmlListGenerator::EXTRAFUNC));

        //load type=other
        if(hlist.contains(ProcessXmlListGenerator::OTHER))
            setOtherDataList(list, hlist.value(ProcessXmlListGenerator::OTHER));
    }

    delete list;
}

bool EditorTab::getCurrentIndexOnlyChecked()
{
    QCheckBox *autoonly;
    switch (this->currentIndex()) {
    case ProcessXmlListGenerator::NORMAL:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox");
        break;
    case ProcessXmlListGenerator::SEARCH:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_2");
        break;
    case ProcessXmlListGenerator::EXTRAFUNC:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_3");
        break;
    case ProcessXmlListGenerator::OTHER:
        autoonly = currentWidget()->findChild<QCheckBox *>("autoOnlyCheckBox_4");
        break;
    default:
        return false;
    }

    return autoonly->isChecked();
}

void EditorTab::tabChanged(int index)
{
    switch(index){
    case ProcessXmlListGenerator::NORMAL:
        editop->editTabAction(currentid, index);
        break;
    case ProcessXmlListGenerator::SEARCH:
        searchcombobox->reloadComboBoxItem();
        localVariantComboBox->reloadComboBoxItem();
        editop->editTabAction(currentid, index);
        break;

    case ProcessXmlListGenerator::EXTRAFUNC:
        extrafunccombobox->reloadComboBoxItem();
        editop->editTabAction(currentid, index);
        break;

    case ProcessXmlListGenerator::OTHER:
        profilecombobox->reloadComboBoxItem();
        editop->editTabAction(currentid, index);
        break;

    default:
        break;
    }
}

void EditorTab::editCheckAction(bool check)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editcheckaction : " << objname;

//"timeoutCheckBox"
//"autoOnlyCheckBox"
//"autoOnlyCheckBox_2"
//"autoOnlyCheckBox_3"
//"autoOnlyCheckBox_4"

    editop->editCheckAction(currentid, check, objname);
}

void EditorTab::editRadioAction(bool)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editradioaction : " << objname;

    if(objname == "variRadioButton"){
        editop->editValueAction(currentid, 0, objname);
    }else if(objname == "fileRadioButton"){
        editop->editValueAction(currentid, 1, objname);
    }
}

void EditorTab::editValueAction(int value)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editvalueaction : " << objname;

    if(objname == "timeoutSpinBox"){
        editop->editValueAction(currentid, value, objname);
    }
}

void EditorTab::editTextAction(QString text)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::edittextaction : " << objname;

    if(objname == "searchComboBox"){
        editop->editSearchComboAction(currentid, text, searchcombobox->currentIndex());
    }else if(objname == "separatorLineEdit"){
        editop->editTextAction(currentid, text, objname);
    }else if(objname == "outputLineEdit"){
        editop->editTextAction(currentid, text, objname);
    }else if(objname == "localVariantComboBox"){
        editop->editComboBoxAction(currentid, text);
    }else if(objname == "extrafuncComboBox"){
        editop->editFileComboAction(currentid, text, extrafunccombobox->getCurrentExtraFile(), objname);
    }else if(objname == "profileComboBox"){
        editop->editFileComboAction(currentid, text, profilecombobox->getCurrentFileName(), objname);
    }
}

void EditorTab::editTableAction(int index, QString str, int function)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::edittableaction : " << objname;
    editop->editTableAction(currentid, index, str, function, objname);

}

void EditorTab::editSwapTableAction(int indexbefore, int indexafter)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editswaptableaction : " << objname;
    editop->swapTableAction(currentid, indexbefore, indexafter, objname);
}

void EditorTab::openSavefile()
{
    QLineEdit *outputLineEdit = this->widget(ProcessXmlListGenerator::SEARCH)->findChild<QLineEdit *>("outputLineEdit");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Search file"),\
                                         QDir::currentPath(), tr("File (*.*)"));

    outputLineEdit->setText(fileName);
}
