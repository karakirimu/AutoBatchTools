/* 
 * Released under the MIT License.
 * See LICENSE text for license details.
 *
 * SPDX-FileCopyrightText: Copyright (c) 2021 karakirimu
 * SPDX-License-Identifier: MIT
 */

#include "editortab.h"

EditorTab::EditorTab(QWidget *parent)
    : QTabWidget(parent)
{
    pluginloader = nullptr;
    plugininstance = nullptr;
}

EditorTab::~EditorTab()
{
    if(pluginloader != nullptr){

        ExtraPluginInterface *ext = qobject_cast<ExtraPluginInterface *>(plugininstance);
        if(ext->getInformation()->hassettingwidget){
            disconnect(ext->getInformation()->settingwidget, \
                       &PluginWidget::sendList, ctableplugins, &PluginCommandTable::insertSettingUpdate);
        }

        pluginloader->unload();
        plugininstance = nullptr;
        delete pluginloader;
    }
}


// Constructor can not initialize connection.
void EditorTab::setConnection()
{
    widgetsearch = this->widget(static_cast<int>(TAB::FILESEARCH));
    addbutton = widgetsearch->findChild<QToolButton *>(SEARCH_ADD);
    editbutton = widgetsearch->findChild<QToolButton *>(SEARCH_EDIT);
    deletebutton = widgetsearch->findChild<QToolButton *>(SEARCH_DELETE);
    searchcombobox = widgetsearch->findChild<SearchComboBox *>(SEARCH_COMBO);
    openToolButton = widgetsearch->findChild<QToolButton *>("openToolButton");

    widgetplugin = this->widget(static_cast<int>(TAB::PLUGINS));
    plugincombobox = widgetplugin->findChild<PluginsComboBox *>("pluginComboBox");
    addbutton_e = widgetplugin->findChild<QToolButton *>("pluginAddToolButton");
    deletebutton_e = widgetplugin->findChild<QToolButton *>("pluginDeleteToolButton");
    pluginsetting = widgetplugin->findChild<QToolButton *>("pluginSettingToolButton");
    scrollSettingWidget = widgetplugin->findChild<QScrollArea *>("scrollArea");
    pluginPane = widgetplugin->findChild<QStackedWidget *>("pluginSetStackedWidget");

    otherwidget = this->widget(static_cast<int>(TAB::PROFILELOAD));
    profilecombobox = otherwidget->findChild<ProfileComboBox *>("profileComboBox");
    addbutton_o = otherwidget->findChild<QToolButton *>("profileAddToolButton");
    deletebutton_o = otherwidget->findChild<QToolButton *>("profileDeleteToolButton");

    //connect action in search widget
    connect(addbutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::addAction);
    connect(editbutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::editAction);
    connect(deletebutton, &QAbstractButton::clicked, searchcombobox, &SearchComboBox::deleteAction);
    connect(openToolButton, &QToolButton::clicked, this, &EditorTab::openSavefile);

    //connect action in plugin widget
    connect(addbutton_e, &QAbstractButton::clicked, plugincombobox, &PluginsComboBox::addItemAction);
    connect(deletebutton_e, &QAbstractButton::clicked, plugincombobox, &PluginsComboBox::deleteAction);
    connect(plugincombobox, &PluginsComboBox::pluginChanged, this, &EditorTab::loadPluginInstanceFromUi);
    connect(pluginsetting, &QPushButton::clicked, this, &EditorTab::pluginSwitchSettingsClicked);

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
    widgetnormal = this->widget(static_cast<int>(TAB::EXECUTE));
    timeoutCheckBox = widgetnormal->findChild<QCheckBox *>("timeoutCheckBox");
    timeoutLineEdit = widgetnormal->findChild<QLineEdit *>("timeoutLineEdit");
    runDetachCheckBox = widgetnormal->findChild<QCheckBox *>("runDetachCheckBox");
    autoonly = widgetnormal->findChild<QCheckBox *>("autoOnlyCheckBox");
    ctablenormal = widgetnormal->findChild<CommandTable *>("cmdTableWidget");

    // load search widget ui objects
    localVariantComboBox = widgetsearch->findChild<VariantComboBox *>("localVariantComboBox");
    outputLineEdit = widgetsearch->findChild<QLineEdit *>("outputLineEdit");
    separatorLineEdit = widgetsearch->findChild<QLineEdit *>("separatorLineEdit");
    autoonly_2 = widgetsearch->findChild<QCheckBox *>("autoOnlyCheckBox_2");
    vari = widgetsearch->findChild<QRadioButton *>("variRadioButton");
    file = widgetsearch->findChild<QRadioButton *>("fileRadioButton");
    fileOverWrite = widgetsearch->findChild<QRadioButton *>("fileOverWriteRadioButton");
    fileAppend = widgetsearch->findChild<QRadioButton *>("fileAppendRadioButton");

    //set editop
    localVariantComboBox->setEditOperator(op);

    // load extrafunc widget ui objects
    autoonly_3 = widgetplugin->findChild<QCheckBox *>("autoOnlyCheckBox_3");
    ctableplugins = widgetplugin->findChild<PluginCommandTable *>("extrafuncTableWidget");

    // load other widget ui objects
    profilecombobox = otherwidget->findChild<ProfileComboBox *>("profileComboBox");
    autoonly_4 = otherwidget->findChild<QCheckBox *>("autoOnlyCheckBox_4");

    //index update
    connect(editop, &EditOperator::processIndexUpdate, this, &EditorTab::setAllIncludeDataList);


    //index edit (table is ignored)
    connect(timeoutCheckBox, &QCheckBox::clicked, this, &EditorTab::editCheckAction);
    connect(runDetachCheckBox, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(timeoutLineEdit, &QLineEdit::textEdited, this, &EditorTab::editTextAction);
    timeoutLineEdit->setValidator(new QIntValidator(0, INT_FAST32_MAX, this));

    connect(autoonly, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(searchcombobox, &SearchComboBox::textActivated, this, &EditorTab::editTextAction);

    connect(separatorLineEdit, &QLineEdit::textEdited, this, &EditorTab::editTextAction);
    connect(localVariantComboBox, &VariantComboBox::textActivated, this, &EditorTab::editTextAction);

    connect(outputLineEdit, &QLineEdit::textEdited, this, &EditorTab::editTextAction);
    connect(autoonly_2, &QCheckBox::clicked, this, &EditorTab::editCheckAction);
    connect(vari, &QRadioButton::clicked, this, &EditorTab::editRadioAction);
    connect(file, &QRadioButton::clicked, this, &EditorTab::editRadioAction);
    connect(fileOverWrite, &QRadioButton::clicked, this, &EditorTab::editRadioAction);
    connect(fileAppend, &QRadioButton::clicked, this, &EditorTab::editRadioAction);

    connect(plugincombobox, &PluginsComboBox::textActivated, this, &EditorTab::editTextAction);

    connect(autoonly_3, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    connect(profilecombobox, &ProfileComboBox::textActivated, this, &EditorTab::editTextAction);
    connect(autoonly_4, &QCheckBox::clicked, this, &EditorTab::editCheckAction);

    //index edit (table only)
    connect(ctablenormal, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(ctablenormal, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
    connect(ctablenormal, &CommandTable::dragDropTable, this, &EditorTab::editDragDropTableAction);

    connect(ctableplugins, &CommandTable::updateTable, this, &EditorTab::editTableAction);
    connect(ctableplugins, &PluginCommandTable::updatePluginWidget, this, &EditorTab::editPluginTableAction);
    connect(ctableplugins, &CommandTable::swapTable, this, &EditorTab::editSwapTableAction);
    connect(ctableplugins, &CommandTable::dragDropTable, this, &EditorTab::editDragDropTableAction);

}

void EditorTab::updateIndex(QString operation)
{
    QStringList sep = operation.split(",");
    int command = sep.last().toInt();

    int rowpos = -1;
    int tablerowpos = -1;

    switch (command) {
    case UiCommandMap::PL_ALLUPDATE_TABLE:
        sep.removeLast();
        ctableplugins->updateTableList(&sep);

        if(pluginloader != nullptr){
            ExtraPluginInterface *ext = qobject_cast<ExtraPluginInterface *>(plugininstance);
            if(ext->getInformation()->hassettingwidget){
                ext->getInformation()->settingwidget->blockSignals(true);
                ext->getInformation()->settingwidget->receiveList(sep);
                ext->getInformation()->settingwidget->blockSignals(false);

            }
        }

        break;

    case UiCommandMap::UNDOREDO_EDIT:
        setAllIncludeDataList(static_cast<QString>(sep.at(0)).toInt());
        break;

    case CommandMap::UNDOREDO_INSERT:
        setAllIncludeDataList(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::E_ADD_TABLE:
        ctablenormal->insertItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::E_INSERT_TABLE:
        ctablenormal->insertItem(static_cast<QString>(sep.at(1)).toInt());
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctablenormal->replaceItem(tablerowpos, getCommandTableRowData(rowpos, tablerowpos, UiCommandMap::E_EDIT_TABLE));
        break;

    case UiCommandMap::E_EDIT_TABLE:
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctablenormal->replaceItem(tablerowpos, getCommandTableRowData(rowpos, tablerowpos, UiCommandMap::E_EDIT_TABLE));
        break;

    case UiCommandMap::E_DELETE_TABLE:
        ctablenormal->deleteItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::E_SWAP_TABLE:
        ctablenormal->swapItem(static_cast<QString>(sep.at(0)).toInt()
                                   , static_cast<QString>(sep.at(1)).toInt());
        break;

    case UiCommandMap::PL_ADD_TABLE:
        ctableplugins->insertItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::PL_INSERT_TABLE:
        ctableplugins->insertItem(static_cast<QString>(sep.at(1)).toInt());
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctableplugins->replaceItem(tablerowpos, getCommandTableRowData(rowpos, tablerowpos, UiCommandMap::PL_EDIT_TABLE));
        break;

    case UiCommandMap::PL_EDIT_TABLE:
        rowpos = static_cast<QString>(sep.at(0)).toInt();
        tablerowpos = static_cast<QString>(sep.at(1)).toInt();
        ctableplugins->replaceItem(tablerowpos, getCommandTableRowData(rowpos, tablerowpos, UiCommandMap::PL_EDIT_TABLE));
        break;

    case UiCommandMap::PL_DELETE_TABLE:
        ctableplugins->deleteItem(static_cast<QString>(sep.at(0)).toInt());
        break;

    case UiCommandMap::PL_SWAP_TABLE:
        ctableplugins->swapItem(static_cast<QString>(sep.at(0)).toInt()
                                    , static_cast<QString>(sep.at(1)).toInt());
        break;

    case UiCommandMap::E_MOVE_TABLE:
        ctablenormal->moveItem(static_cast<QString>(sep.at(1)).toInt()
                                   , static_cast<QString>(sep.at(2)).toInt()
                                   , static_cast<QString>(sep.at(3)).toInt());
        break;

    case UiCommandMap::PL_MOVE_TABLE:
        ctableplugins->moveItem(static_cast<QString>(sep.at(1)).toInt()
                                    , static_cast<QString>(sep.at(2)).toInt()
                                    , static_cast<QString>(sep.at(3)).toInt());
        break;

    default:
        break;
    }
}

/**
 * @brief EditorTab::setAllIncludeDataList
 * @param itemid
 * @param unused1
 * @param unused2
 */
void EditorTab::setAllIncludeDataList(int itemid, int unused1, int unused2)
{
    Q_UNUSED(unused1)
    Q_UNUSED(unused2)

    EditorCache list;
    FunctionType ft;

    //avoid multiple update
    if(itemid < 2 || !editop->read(itemid, &list)){
        return;
    }

    //set current id
    qDebug() << "[EditorTab::setCombinedDataList]     currentid update : " << itemid;
    currentid = itemid;

    //set widget selection
    this->blockSignals(true);

    switch (ft.getType(list.type)) {
    case ft.TYPE::ALLINCLUDE:
        setCurrentIndex(list.functionSelect);
        setExecuteDataList(&list);
        setFileSearchDataList(&list);
        setPluginDataList(&list);
        setProfileLoadDataList(&list);
        break;
    case ft.TYPE::INFORMATION: break;
    case ft.TYPE::LOCAL:       break;
    case ft.TYPE::EXECUTE:     setCurrentIndex(list.functionSelect); setExecuteDataList(&list);     break;
    case ft.TYPE::FILESEARCH:  setCurrentIndex(list.functionSelect); setFileSearchDataList(&list);  break;
    case ft.TYPE::PLUGIN:      setCurrentIndex(list.functionSelect); setPluginDataList(&list);      break;
    case ft.TYPE::PROFILELOAD: setCurrentIndex(list.functionSelect); setProfileLoadDataList(&list); break;
    case ft.TYPE::INVALID: break;
    }

    this->blockSignals(false);
}

/**
 * @brief EditorTab::setExecuteDataList
 * @param list
 */
void EditorTab::setExecuteDataList(EditorCache *list)
{
    int counter = list->exec.command.count();
    this->blockSignals(true);

    runDetachCheckBox->setChecked(list->exec.detach);

    timeoutCheckBox->setChecked(list->exec.timeoutEnabled);
    timeoutLineEdit->setText(QString::number(list->exec.timeout));

    ctablenormal->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        ctablenormal->replaceItem(i, list->exec.command.at(i));
    }

    autoonly->setChecked(list->exec.schedulerOnly);

    this->blockSignals(false);
}

/**
 * @brief EditorTab::setFileSearchDataList
 * @param list
 */
void EditorTab::setFileSearchDataList(EditorCache *list)
{
    this->blockSignals(true);

    searchcombobox->reloadComboBoxItem();
    searchcombobox->setCurrentText(list->filesearch.name);

    separatorLineEdit->setText(list->filesearch.separator);

    localVariantComboBox->reloadComboBoxItem();
    localVariantComboBox->setCurrentText(list->filesearch.variant);
    outputLineEdit->setText(list->filesearch.filePath);

    if(list->filesearch.outputOption == list->filesearch.VARIANT){
        vari->setChecked(true);
    }else{
        file->setChecked(true);
    }

    if(list->filesearch.writeOption == list->filesearch.OVERWRITE){
        fileOverWrite->setChecked(true);
    }else{
        fileAppend->setChecked(true);
    }

    autoonly_2->setChecked(list->filesearch.schedulerOnly);

    this->blockSignals(false);
}

/**
 * @brief EditorTab::setPluginDataList
 * @param list
 */
void EditorTab::setPluginDataList(EditorCache *list)
{
    this->blockSignals(true);
    plugincombobox->blockSignals(true);

    //reset combobox
    plugincombobox->reloadComboBoxItem();

    // update table
    int counter = list->plugin.command.count();

    ctableplugins->setRowCount(counter);
    for(int i = 0; i < counter; i++){
        ctableplugins->replaceItem(i, list->plugin.command.at(i));
    }

    //get plugin name
    QString plfile = list->plugin.filePath;

    plugincombobox->setComboBoxItem(&plfile);

    loadPluginInstance(plfile);

    if(plugininstance != nullptr){
        ExtraPluginInterface *ext = qobject_cast<ExtraPluginInterface *>(plugininstance);
        plugincombobox->setCurrentText(ext->getInformation()->name);
    }

    autoonly_3->setChecked(list->plugin.schedulerOnly);

    this->blockSignals(false);
    plugincombobox->blockSignals(false);
}

/**
 * @brief EditorTab::setProfileLoadDataList
 * @param list
 */
void EditorTab::setProfileLoadDataList(EditorCache *list)
{
    this->blockSignals(true);

    QString curdata = list->profileload.filePath;
    QFileInfo profile(curdata);

    if(profile.exists()){
        //read file
        ProcessXmlBuilder tpxb;
        QList<QStringList> tlist;
        tpxb.setLoadPath(curdata);

        if(tpxb.readItem(0, &tlist)){
            ProcessXmlConstant pxc;
            curdata = xgen.fetch(pxc.TAG_I_NAME, &tlist);
        }

    }else{
        curdata = tr("(no file)");
    }

    profilecombobox->reloadComboBoxItem();
    profilecombobox->setIndex(profile.canonicalFilePath());

    autoonly_4->setChecked(list->profileload.schedulerOnly);

    this->blockSignals(false);
}

/**
 * @brief EditorTab::getCommandTableRowData
 * @param targetrow
 * @param tablerow
 * @param loadtype
 * @return
 */
QString EditorTab::getCommandTableRowData(int targetrow, int tablerow, int loadtype)
{
    EditorCache list;

    if(targetrow < 2 || !editop->read(targetrow, &list)){
        qDebug() << "[EditorTab::getCommandTableRowData] EditorCache read failed.";
        return "";
    }

    if(loadtype == UiCommandMap::E_EDIT_TABLE){
        return list.exec.command.at(tablerow);

    }else{
        return list.plugin.command.at(tablerow);
    }
}

// DOUBT
void EditorTab::tabChanged(int index)
{
    switch(index){
    case static_cast<int>(TAB::EXECUTE):

        editop->editTabAction(currentid, index);
        break;

    case static_cast<int>(TAB::FILESEARCH):
        this->blockSignals(true);
        searchcombobox->reloadComboBoxItem();
        localVariantComboBox->reloadComboBoxItem();
        this->blockSignals(false);

        editop->editTabAction(currentid, index);
        break;

    case static_cast<int>(TAB::PLUGINS):
        this->blockSignals(true);
        plugincombobox->reloadComboBoxItem();
        this->blockSignals(false);

        editop->editTabAction(currentid, index);
        break;

    case static_cast<int>(TAB::PROFILELOAD):
        this->blockSignals(true);
        profilecombobox->reloadComboBoxItem();
        this->blockSignals(false);

        editop->editTabAction(currentid, index);
        break;

    default:
        break;
    }
}

/**
 * @fn EditorTab::editCheckAction
 * @brief
 *  "timeoutCheckBox"
 *  "autoOnlyCheckBox" normal
 *  "autoOnlyCheckBox_2" search
 *  "autoOnlyCheckBox_3" plugin
 *  "autoOnlyCheckBox_4" other
 * @param check checkbox status
 */
void EditorTab::editCheckAction(bool check)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::editcheckaction : " << objname;

    if(objname == "timeoutCheckBox"){
        editop->checkTimeoutAction(currentid, check);

    }else if(objname == "runDetachCheckBox"){
        editop->checkRunDetachAction(currentid, check);

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

    }else if(objname == "fileOverWriteRadioButton"){
        editop->radioSearchFileOutputTypeAction(currentid, 0);

    }else if(objname == "fileAppendRadioButton"){
        editop->radioSearchFileOutputTypeAction(currentid, 1);

    }
}

void EditorTab::editTextAction(QString text)
{
    QString objname = this->sender()->objectName();
    qDebug() << "EditorTab::edittextaction : " << objname;

    if(objname == SEARCH_COMBO){
        editop->comboboxSearchAction(currentid
                                     , text
                                     , searchcombobox->currentIndex());

    }else if(objname == "separatorLineEdit"){
        editop->textSearchSeparateAction(currentid, text);

    }else if(objname == "outputLineEdit"){
        editop->textFileOutputAction(currentid, text);

    }else if(objname == "localVariantComboBox"){
        editop->comboboxLocalValAction(currentid, text);

    }else if(objname == "pluginComboBox"){
        editop->comboboxPluginAction(currentid
                                     , text
                                     , plugincombobox->getCurrentExtraFile());

    }else if(objname == "profileComboBox"){
        editop->comboboxProfileAction(currentid
                                      , text
                                      , profilecombobox->getCurrentFileName());

    }else if(objname == "timeoutLineEdit"){
        editop->spinTimeoutAction(currentid, text.toInt());
    }
}

void EditorTab::editTableAction(int index, QString str, int function)
{
    QString objname = this->sender()->objectName();
    qDebug() << "[EditorTab::editTableAction] object : " << objname;

    if(objname == "cmdTableWidget"){
        qDebug() << "[EditorTab::editTableAction] currentid:"
                 << currentid
                 << " index: " << index
                 << " str: " << str
                 << " function: " << function;

        editop->tableEditExecAction(currentid, index, str, function);

    }else if(objname == "extrafuncTableWidget"){
        editop->tableEditPluginAction(currentid, index, str, function);

    }
}

void EditorTab::editPluginTableAction(QStringList strlist, int function)
{
    QString objname = this->sender()->objectName();
    qDebug() << "[EditorTab::editPluginTableAction] object : " << objname;

    if(objname == "extrafuncTableWidget"){
        editop->tableEditPluginAction(currentid, strlist, function);

    }
}

void EditorTab::editSwapTableAction(int indexbefore, int indexafter)
{
    QString objname = this->sender()->objectName();
    qDebug() << "[EditorTab::editswaptableaction] object : " << objname;

    if(objname == "cmdTableWidget"){
        editop->tableSwapExecAction(currentid, indexbefore, indexafter);

    }else if(objname == "extrafuncTableWidget"){
        editop->tableSwapPluginAction(currentid, indexbefore, indexafter);

    }
}

void EditorTab::editDragDropTableAction(QList<int> indexbefore, int indexafter)
{
    QString objname = this->sender()->objectName();
    qDebug() << "[EditorTab::editDragDropTableAction] object : " << objname;

    if(objname == "cmdTableWidget"){
        editop->tableDragDropExecAction(currentid, indexbefore, indexafter);

    }else if(objname == "extrafuncTableWidget"){
        editop->tableDragDropPluginAction(currentid, indexbefore, indexafter);

    }
}

/**
 * @fn EditorTab::pluginSwitchSettingsClicked
 * @brief Switch between the command widget and the settings widget
 *        in the plugin tab. Operation is enabled only when hassettingwidget
 *        is true when loading plugins.
 */
void EditorTab::pluginSwitchSettingsClicked()
{
    if(pluginPane->currentIndex() == PluginPane::SettingWidget){
        pluginPane->setCurrentIndex(PluginPane::Command);
    }else{
        pluginPane->setCurrentIndex(PluginPane::SettingWidget);
    }
}

void EditorTab::loadPluginInstanceFromUi(QString plfile)
{
    if(pluginloader != nullptr){
        ctableplugins->setRowCount(0);
    }

    loadPluginInstance(plfile);
}

/**
 * @fn EditorTab::openSavefile
 * @brief Select an output file in FileSearch tab widget.
 */
void EditorTab::openSavefile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save FileSearch Result"),\
                                         QDir::currentPath(), "File (*.*)");

    if(fileName != ""){
        outputLineEdit->setText(fileName);
        emit outputLineEdit->textEdited(fileName);
    }
}

/**
 * @fn EditorTab::loadPluginInstance
 * @brief Load the plugin. The instance is retained until you switch
 *        the row selection in ProcessFlowTable.
 * @param plfile File path where the plugin exists.
 */
void EditorTab::loadPluginInstance(QString plfile)
{
    ExtraPluginInterface *ext = nullptr;

    if(pluginloader != nullptr){

        ext = qobject_cast<ExtraPluginInterface *>(plugininstance);
        if(ext->getInformation()->hassettingwidget){
            disconnect(ext->getInformation()->settingwidget, &PluginWidget::sendList, \
                       ctableplugins, &PluginCommandTable::insertSettingUpdate);
        }


        pluginloader->unload();

        delete pluginloader;
        pluginloader = nullptr;
        plugininstance = nullptr;
        ext = nullptr;
    }

    QFileInfo info(plfile);

    if(info.exists()){
        //check plugin can use
        pluginloader = new QPluginLoader(plfile);

        if(pluginloader->load()){
            plugininstance = pluginloader->instance();
            ext = qobject_cast<ExtraPluginInterface *>(plugininstance);

        }else{
            delete pluginloader;
            pluginloader = nullptr;
            plugininstance = nullptr;
        }
    }

    // update gui widget
    if(ext != nullptr){
        if(ext->getInformation()->hassettingwidget){

            // get table items
            QStringList tmp;
            int tc = ctableplugins->rowCount();

            for (int i = 0; i < tc; i++) {
                tmp.append(ctableplugins->item(i, 0)->text());
            }

            // set current table items (set first for later signal avoid)
            ext->getInformation()->settingwidget->receiveList(tmp);

            PluginWidget *pw = ext->getInformation()->settingwidget;
            pw->setPalette(this->pluginPane->palette());

            scrollSettingWidget->setWidget(pw);

            // connect to ctableplugin
            connect(ext->getInformation()->settingwidget, &PluginWidget::sendList, \
                       ctableplugins, &PluginCommandTable::insertSettingUpdate);

            pluginsetting->setEnabled(true);
            pluginPane->setCurrentIndex(PluginPane::SettingWidget);
        }else{
            pluginsetting->setEnabled(false);
            pluginPane->setCurrentIndex(PluginPane::Command);
        }

    }else{
        scrollSettingWidget->takeWidget();
        pluginsetting->setEnabled(false);
        pluginPane->setCurrentIndex(PluginPane::SettingWidget);
    }
}
