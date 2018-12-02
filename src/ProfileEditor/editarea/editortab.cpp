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
    extrafunccombobox = widgetextra->findChild<PluginsComboBox *>("extrafuncComboBox");
    addbutton_e = widgetextra->findChild<QToolButton *>("extrafuncAddButton");
    deletebutton_e = widgetextra->findChild<QToolButton *>("extrafuncDeleteButton");
    pluginsetting = widgetextra->findChild<QToolButton *>("pluginSettingButton");

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
    connect(addbutton_e, &QAbstractButton::clicked, extrafunccombobox, &PluginsComboBox::addItemAction);
    connect(deletebutton_e, &QAbstractButton::clicked, extrafunccombobox, &PluginsComboBox::deleteAction);
    connect(extrafunccombobox, &PluginsComboBox::pluginChanged, pluginsetting, &QPushButton::setEnabled);
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
    timeoutLineEdit = widgetnormal->findChild<QLineEdit *>("timeoutLineEdit");
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
    ctableplugins = widgetextra->findChild<CommandTable *>("extrafuncTableWidget");

    // load other widget ui objects
    profilecombobox = otherwidget->findChild<ProfileComboBox *>("profileComboBox");
    autoonly_4 = otherwidget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    //index update
//    connect(editop, &EditOperator::ui_selectindexUpdate, this, &EditorTab::setCombinedDataList);
    connect(editop, &EditOperator::ui_funcindexUpdate, this, &EditorTab::setCombinedDataList);


    //index edit (table is ignored)
    connect(timeoutCheckBox, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(timeoutLineEdit, &QLineEdit::textEdited, this, &EditorTab::editTextAction);
    timeoutLineEdit->setValidator(new QIntValidator(0, INT_FAST32_MAX, this));

    connect(autoonly, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(searchcombobox, QOverload<const QString &>::of(&SearchComboBox::activated) \
            , this, &EditorTab::editTextAction);

    connect(separatorLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(localVariantComboBox, QOverload<const QString &>::of(&VariantComboBox::activated) \
            , this, &EditorTab::editTextAction);

    connect(outputLineEdit, &QLineEdit::textChanged, this, &EditorTab::editTextAction);
    connect(autoonly_2, &QCheckBox::clicked, this, &EditorTab::editCheckAction);
    connect(vari, &QRadioButton::clicked, this, &EditorTab::editRadioAction);
    connect(file, &QRadioButton::clicked, this, &EditorTab::editRadioAction);

    connect(extrafunccombobox, QOverload<const QString &>::of(&PluginsComboBox::activated) \
            , this, &EditorTab::editTextAction);

    connect(autoonly_3, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(profilecombobox, &ProfileComboBox::currentTextChanged, this, &EditorTab::editTextAction);
    connect(autoonly_4, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    //index edit (table only)
    connect(ctablenormal, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(ctablenormal, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
    connect(ctableplugins, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(ctableplugins, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
}

void EditorTab::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");

    if(sep.count() < 2){
        //edit
        setCombinedDataList(static_cast<QString>(sep.at(0)).toInt(), -1, \
                            EditOperator::SELECT, EditOperator::MAINEDITOR);

//    }else if(sep.at(1) == UNDOREDO_ADD){
//        //add

//    }else if(sep.at(1) == UNDOREDO_DELETE){
//        //del

    }else if(sep.at(1) == UNDOREDO_INSERT){
        //ins
        setCombinedDataList(static_cast<QString>(sep.at(0)).toInt(), -1, \
                            EditOperator::SELECT, EditOperator::MAINEDITOR);

    }else if(sep.at(1) == UNDOREDO_E_TABLEADD){
        //exectableadd
        ctablenormal->insertItem(static_cast<QString>(sep.at(0)).toInt());

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_E_TABLEINS){
        //exectableins
        ctablenormal->insertItem(static_cast<QString>(sep.at(1)).toInt());
        int rowpos = static_cast<QString>(sep.at(0)).toInt();
        int tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctablenormal->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos, UNDOREDO_E_TABLEEDIT));

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_E_TABLEEDIT){
        //exectableedit
        int rowpos = static_cast<QString>(sep.at(0)).toInt();
        int tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctablenormal->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos, UNDOREDO_E_TABLEEDIT));

    }else if(sep.at(1) == UNDOREDO_E_TABLEDEL){
        //exectabledel
        ctablenormal->deleteItem(static_cast<QString>(sep.at(0)).toInt());

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_E_TABLESWAP){
        //exectableswap
        ctablenormal->swapItem(static_cast<QString>(sep.at(0)).toInt()
                               , static_cast<QString>(sep.at(1)).toInt());

    }else if(sep.at(1) == UNDOREDO_PL_TABLEADD){
        //exectableadd
        ctableplugins->insertItem(static_cast<QString>(sep.at(0)).toInt());

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_PL_TABLEINS){
        //exectableins
        ctableplugins->insertItem(static_cast<QString>(sep.at(1)).toInt());
        int rowpos = static_cast<QString>(sep.at(0)).toInt();
        int tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctableplugins->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos, UNDOREDO_PL_TABLEEDIT));

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_PL_TABLEEDIT){
        //exectableedit
        int rowpos = static_cast<QString>(sep.at(0)).toInt();
        int tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctableplugins->replaceItem(tablerowpos, getTableData(rowpos, tablerowpos, UNDOREDO_PL_TABLEEDIT));

    }else if(sep.at(1) == UNDOREDO_PL_TABLEDEL){
        //exectabledel
        ctableplugins->deleteItem(static_cast<QString>(sep.at(0)).toInt());

    }else if(sep.count() == 3
             && sep.at(2) == UNDOREDO_PL_TABLESWAP){
        //exectableswap
        ctableplugins->swapItem(static_cast<QString>(sep.at(0)).toInt()
                               , static_cast<QString>(sep.at(1)).toInt());

    }
}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setNormalDataList(QList<QStringList> *list)
{
    int counter = xgen.fetch(E_CMDARGCOUNT,ATTR_NONE, list).toInt();
    this->blockSignals(true);

    timeoutCheckBox->setChecked(VariantConverter::stringToBool(xgen.fetch(E_TIMEOUT,ATTR_NONE, list)));


    timeoutLineEdit->setText(xgen.fetch(E_TIMEOUT,ATTR_TIMEOUTMS, list));

    int cmdfirst = xgen.fetchCmdFirstPos(E_CMD, list);

    ctablenormal->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        ctablenormal->setItem(i, 0, new QTableWidgetItem(list->at(cmdfirst + i).at(1)));
    }

    autoonly->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_EXEC,ATTR_ONLY_SCHEDULER, list)));

    this->blockSignals(false);

}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setSearchDataList(QList<QStringList> *list)
{
    this->blockSignals(true);

    searchcombobox->reloadComboBoxItem();
    searchcombobox->setCurrentText(xgen.fetch(S_NAME,ATTR_NONE, list));

    separatorLineEdit->setText(xgen.fetch(S_SEPARATOR,ATTR_NONE, list));
    localVariantComboBox->insertItem(0,xgen.fetch(S_VARIANT,ATTR_NONE, list));
    outputLineEdit->setText(xgen.fetch(S_OUTPUTFILE,ATTR_NONE, list));

    if(xgen.fetch(S_OUTPUTFILE,ATTR_RADIOBUTTONPOS, list) == "0"){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    autoonly_2->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_SEARCH,ATTR_ONLY_SCHEDULER, list)));

    this->blockSignals(false);

}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setPluginDataList(QList<QStringList> *list)
{
    this->blockSignals(true);

    //reset combobox
    extrafunccombobox->reloadComboBoxItem();
    extrafunccombobox->setCurrentText(xgen.fetch(PL_NAME,ATTR_NONE, list));

    int counter = xgen.fetch(PL_CMDARGCOUNT,ATTR_NONE, list).toInt();
    int ecmdfirst = xgen.fetchCmdFirstPos(PL_CMD, list);
    ctableplugins->setRowCount(counter);

    for(int i = 0; i < counter; i++){
        ctableplugins->setItem(i, 0, new QTableWidgetItem(list->at(ecmdfirst + i).at(1)));
    }

    autoonly_3->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_SCRIPT,ATTR_ONLY_SCHEDULER, list)));

    this->blockSignals(false);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setOtherDataList(QList<QStringList> *list)
{
    this->blockSignals(true);

    profilecombobox->reloadComboBoxItem();
    profilecombobox->setCurrentText(xgen.fetch(PR_NAME,ATTR_NONE, list));

    autoonly_4->setChecked(VariantConverter::stringToBool(xgen.fetch(TYPE_ANOTHER,ATTR_ONLY_SCHEDULER, list)));

    this->blockSignals(false);
}

///DEPENDS_XML DEPENDS_UI PROCESS
void EditorTab::setCombinedDataList(int after, int before, int function, int sendfrom)
{
    Q_UNUSED(before); Q_UNUSED(sendfrom); Q_UNUSED(function)

    QList<QStringList> *list = new QList<QStringList>();

    //avoid multiple update
//    if(function == EditOperator::EDIT){
//        if(after < 2 || !editop->read(after, list)){
//            delete list;
//            return;
//        }
//    }else{
        if(/*currentid == after ||*/ after < 2 || !editop->read(after, list)){
            delete list;
            return;
        }
//    }

    //set current id
    qDebug() << QString("EditorTab : currentid update to %1").arg(after);
    currentid = after;

    //set widget selection
    this->blockSignals(true);

    QString type = xgen.fetch(ALL_TYPE, ATTR_NONE, list);
    if(type == TYPE_ALLINCLUDE){
        setCurrentIndex(static_cast<QString>(xgen.fetch(TE_STACKEDWIDGET_POSITION, ATTR_NONE, list)).toInt());

        setNormalDataList(list);
        setSearchDataList(list);
        setPluginDataList(list);
        setOtherDataList(list);

    }else if(type == TYPE_EXEC){
        setNormalDataList(list);

    }else if(type == TYPE_SEARCH){
        setSearchDataList(list);

    }else if(type == TYPE_SCRIPT){
        setPluginDataList(list);

    }else if(type == TYPE_ANOTHER){
        setOtherDataList(list);
    }

    this->blockSignals(false);

    delete list;
}

QString EditorTab::getTableData(int targetrow, int tablerow, QString loadtype)
{
    QList<QStringList> *list = new QList<QStringList>();

    if(targetrow < 2 || !editop->read(targetrow, list)){
        delete list;
        return "";
    }

    int cmdfirst = 0;

    if(loadtype == UNDOREDO_E_TABLEEDIT){
        cmdfirst = xgen.fetchCmdFirstPos(E_CMD, list);
    }else{
        cmdfirst = xgen.fetchCmdFirstPos(PL_CMD, list);
    }

    return list->at(cmdfirst + tablerow).at(1);
}

// DOUBT
void EditorTab::tabChanged(int index)
{
    switch(index){
    case ProcessXmlListGenerator::NORMAL:

        editop->editTabAction(currentid, index);
        break;

    case ProcessXmlListGenerator::SEARCH:
        this->blockSignals(true);
        searchcombobox->reloadComboBoxItem();
        localVariantComboBox->reloadComboBoxItem();
        this->blockSignals(false);

        editop->editTabAction(currentid, index);
        break;

    case ProcessXmlListGenerator::EXTRAFUNC:
        this->blockSignals(true);
        extrafunccombobox->reloadComboBoxItem();
        this->blockSignals(false);

        editop->editTabAction(currentid, index);
        break;

    case ProcessXmlListGenerator::OTHER:
        this->blockSignals(true);
        profilecombobox->reloadComboBoxItem();
        this->blockSignals(false);

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

//void EditorTab::editValueAction(QString value)
//{
//    QString objname = this->sender()->objectName();
//    qDebug() << "EditorTab::editvalueaction : " << objname;

//    if(objname == "timeoutLineEdit"){
//        editop->spinTimeoutAction(currentid, value.toInt());

//    }
//}

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

    }else if(objname == "timeoutLineEdit"){
        editop->spinTimeoutAction(currentid, text.toInt());
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

}

void EditorTab::editSwapTableAction(int indexbefore, int indexafter)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editswaptableaction : " << objname;
    if(objname == "cmdTableWidget"){
        editop->tableSwapExecAction(currentid, indexbefore, indexafter);

    }else if(objname == "ctableplugins"){
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
        int excount = ctableplugins->rowCount();
        QStringList currentargs;
        for(int i = 0; i < excount; i++){
            currentargs.append(ctableplugins->model()->index(i, 0).data().toString());
        }

        inter->launchSettingWidget(&currentargs, &resultargs, \
                                   editop->getMainWindowGeometry().center(), \
                                   extrafunccombobox->styleSheet());

        //write data
        ctableplugins->insertItems(&resultargs);

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
