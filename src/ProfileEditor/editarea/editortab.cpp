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
    addbutton = widgetsearch->findChild<QToolButton *>(SEARCH_ADD);
    editbutton = widgetsearch->findChild<QToolButton *>(SEARCH_EDIT);
    deletebutton = widgetsearch->findChild<QToolButton *>(SEARCH_DELETE);
    searchcombobox = widgetsearch->findChild<SearchComboBox *>(SEARCH_COMBO);
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
    connect(pluginsetting, &QPushButton::clicked, this, &EditorTab::pluginSettingsClicked);

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

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setNormalDataList(QList<QStringList> *list, int firstpos)
{
    this->blockSignals(true);

//    int counter = static_cast<QString>(list->at(firstpos + 2).at(1)).toInt();
//    timeoutCheckBox->setChecked(VariantConverter::stringToBool(list->at(firstpos + 1).at(1)));
//    tospin->setValue(static_cast<QString>(list->at(firstpos + 1).at(3)).toInt());
//    ctablenormal->setRowCount(counter);
//    for(int i = 0; i < counter; i++){
//        ctablenormal->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 3 + i).at(1)));
//    }

//    autoonly->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    int counter = xgen.fetch(E_CMDARGCOUNT,ATTR_NONE, list, firstpos).toInt();
    timeoutCheckBox->setChecked(VariantConverter::stringToBool(xgen.fetch(E_TIMEOUT,ATTR_NONE, list, firstpos)));
    tospin->setValue(xgen.fetch(E_TIMEOUT,ATTR_TIMEOUTMS, list, firstpos).toInt());
    ctablenormal->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        ctablenormal->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 3 + i).at(1)));
    }

    autoonly->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_EXEC,ATTR_ONLY_SCHEDULER, list, firstpos)));

    this->blockSignals(false);

}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setSearchDataList(QList<QStringList> *list, int firstpos)
{
    this->blockSignals(true);

    //reload combobox
//    searchcombobox->reloadComboBoxItem();
//    searchcombobox->setCurrentText(list->at(firstpos + 1).at(1));

//    separatorLineEdit->setText(list->at(firstpos + 2).at(1));
//    localVariantComboBox->insertItem(0,list->at(firstpos + 3).at(1));
//    outputLineEdit->setText(list->at(firstpos + 4).at(1));

//    if(list->at(firstpos + 4).at(3) == "0"){
//        vari->setChecked(true);
//    }else{
//        file->setChecked(true);
//    }

//    autoonly_2->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    searchcombobox->reloadComboBoxItem();
    searchcombobox->setCurrentText(xgen.fetch(S_NAME,ATTR_NONE, list, firstpos));

    separatorLineEdit->setText(xgen.fetch(S_SEPARATOR,ATTR_NONE, list, firstpos));
    localVariantComboBox->insertItem(0,xgen.fetch(S_VARIANT,ATTR_NONE, list, firstpos));
    outputLineEdit->setText(xgen.fetch(S_OUTPUTFILE,ATTR_NONE, list, firstpos));

    if(xgen.fetch(S_OUTPUTFILE,ATTR_RADIOBUTTONPOS, list, firstpos) == "0"){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    autoonly_2->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_SEARCH,ATTR_ONLY_SCHEDULER, list, firstpos)));

    this->blockSignals(false);

}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setExtraFuncDataList(QList<QStringList> *list, int firstpos)
{
    this->blockSignals(true);
//    //reset combobox
//    extrafunccombobox->reloadComboBoxItem();
//    extrafunccombobox->setCurrentText(list->at(firstpos + 1).at(1));

//    int counter = static_cast<QString>(list->at(firstpos + 3).at(1)).toInt();
//    extrafuncTableWidget->setRowCount(counter);
//    for(int i = 0; i < counter; i++){
//        extrafuncTableWidget->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 4 + i).at(1)));
//    }

//    autoonly_3->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    // TODO:動的検索に最初の要素を追加

    //reset combobox
    extrafunccombobox->reloadComboBoxItem();
    extrafunccombobox->setCurrentText(xgen.fetch(PL_NAME,ATTR_NONE, list, firstpos));

    int counter = xgen.fetch(PL_CMDARGCOUNT,ATTR_NONE, list, firstpos).toInt();
    extrafuncTableWidget->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        extrafuncTableWidget->setItem(i, 0, new QTableWidgetItem(list->at(firstpos + 4 + i).at(1)));
    }

    autoonly_3->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_SCRIPT,ATTR_ONLY_SCHEDULER, list, firstpos)));

    this->blockSignals(false);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setOtherDataList(QList<QStringList> *list, int firstpos)
{
    this->blockSignals(true);

//    profilecombobox->reloadComboBoxItem();
//    profilecombobox->setCurrentText(list->at(firstpos + 1).at(1));

//    autoonly_4->setChecked(VariantConverter::stringToBool(list->at(firstpos).at(3)));

    profilecombobox->reloadComboBoxItem();
    profilecombobox->setCurrentText(xgen.fetch(PR_NAME,ATTR_NONE, list, firstpos));

    autoonly_4->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_ANOTHER,ATTR_ONLY_SCHEDULER, list, firstpos)));

    this->blockSignals(false);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setCombinedDataList(int index, int selectfrom)
{
    Q_UNUSED(selectfrom)
    QList<QStringList> *list = new QList<QStringList>();
    QHash<int, int> hlist;

    //avoid multiple update
    if(editop->read(index, list) && currentid != index){

        //set current id
        qDebug() << QString("EditorTab : currentid update to %1").arg(index);
        currentid = index;
        xgen.getListStructure(list, &hlist);

        //no file showing module
    //    int cmdskip = (QString)(list->at(sfunction->firstPosNormal() + 1).at(1)).toInt();

        //set widget selection
        if(hlist.count() > 1){
            disconnect(this, &EditorTab::currentChanged, this, &EditorTab::tabChanged);
            setCurrentIndex(static_cast<QString>(list->at(1).at(1)).toInt());
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
//"autoOnlyCheckBox" normal
//"autoOnlyCheckBox_2" search
//"autoOnlyCheckBox_3" extrafunc
//"autoOnlyCheckBox_4" other
    if(objname == "timeoutCheckBox"){
        editop->checkTimeoutAction(currentid, check);

    }else if(objname == "autoOnlyCheckBox"){
        editop->checkOnlyNormalAction(currentid, check);

    }else if(objname == "autoOnlyCheckBox_2"){
        editop->checkOnlySearchAction(currentid, check);

    }else if(objname == "autoOnlyCheckBox_3"){
        editop->checkOnlyPluginAction(currentid, check);

    }else if(objname == "autoOnlyCheckBox_4"){
        editop->checkOnlyOtherAction(currentid, check);

    }

}

void EditorTab::editRadioAction(bool)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editradioaction : " << objname;

    if(objname == "variRadioButton"){
        editop->radioSearchOutputAction(currentid, 0);

    }else if(objname == "fileRadioButton"){
        editop->radioSearchOutputAction(currentid, 1);

    }
}

void EditorTab::editValueAction(int value)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editvalueaction : " << objname;

    if(objname == "timeoutSpinBox"){
        editop->spinTimeoutAction(currentid, value);

    }
}

void EditorTab::editTextAction(QString text)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::edittextaction : " << objname;

    if(objname == SEARCH_COMBO){
        editop->comboboxSearchAction(currentid, text, searchcombobox->currentIndex());

    }else if(objname == "separatorLineEdit"){
        editop->textSearchSeparateAction(currentid, text);

    }else if(objname == "outputLineEdit"){
        editop->textFileOutputAction(currentid, text);

    }else if(objname == "localVariantComboBox"){
        editop->comboboxLocalValAction(currentid, text);

    }else if(objname == "extrafuncComboBox"){
        editop->comboboxPluginAction(currentid, text, extrafunccombobox->getCurrentExtraFile());

    }else if(objname == "profileComboBox"){
        editop->comboboxProfileAction(currentid, text, profilecombobox->getCurrentFileName());

    }
}

void EditorTab::editTableAction(int index, QString str, int function)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::edittableaction : " << objname;
    if(objname == "cmdTableWidget"){
        editop->tableEditExecAction(currentid, index, str, function);
    }else if(objname == "extrafuncTableWidget"){
        editop->tableEditPluginAction(currentid, index, str, function);
    }
//    editop->editTableAction(currentid, index, str, function, objname);

}

void EditorTab::editSwapTableAction(int indexbefore, int indexafter)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editswaptableaction : " << objname;
    if(objname == "cmdTableWidget"){
        editop->tableSwapExecAction(currentid, indexbefore, indexafter);
    }else if(objname == "extrafuncTableWidget"){
        editop->tableSwapPluginAction(currentid, indexbefore, indexafter);
    }
}

void EditorTab::pluginSettingsClicked()
{
    QPluginLoader loader(extrafunccombobox->getCurrentExtraFile());
    if(loader.load()){
        QObject *plugin = loader.instance();
        ExtraPluginInterface *inter = qobject_cast<ExtraPluginInterface *>(plugin);
        QStringList resultargs;

        //read data
        int excount = extrafuncTableWidget->rowCount();
        QStringList currentargs;
        for(int i = 0; i < excount; i++){
            currentargs.append(extrafuncTableWidget->model()->index(i, 0).data().toString());
        }

        inter->launchSettingWidget(&currentargs, &resultargs, \
                                   editop->getMainWindowGeometry().center(), \
                                   extrafunccombobox->styleSheet());

        //write data
        extrafuncTableWidget->insertItems(&resultargs);

        loader.unload();
    }
}

void EditorTab::openSavefile()
{
    QLineEdit *outputLineEdit = this->widget(ProcessXmlListGenerator::SEARCH)->findChild<QLineEdit *>("outputLineEdit");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Search file"),\
                                         QDir::currentPath(), tr("File (*.*)"));

    outputLineEdit->setText(fileName);
}
