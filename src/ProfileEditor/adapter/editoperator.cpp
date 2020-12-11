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

#include "editoperator.h"

EditOperator::EditOperator(QObject *parent)
    : QObject(parent)
{
    undostack = new QUndoStack();
    editorCache = new EditorCacheList();

    parentwid = qobject_cast<QMainWindow *>(parent);

    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );

    //init timer (ms)
    settings.beginGroup(sc.GROUP_ABE);
    timerid = startTimer(settings.value(sc.ABE_AUTOSAVE_PERIOD, 1).toInt() * 60000);
    settings.endGroup();
}

EditOperator::~EditOperator()
{
    if(autosavefile != ""){
        QFile file(autosavefile);
        if(!file.remove()){
            qDebug() << file.errorString() << file.isOpen();
        }
    }

    delete editorCache;
}

/**
 * @fn EditOperator::read
 * @brief Read cache data corresponding to id
 * @param id   Row number to read, starting from 0
 * @param list Pointer to store the result
 * @return Returns true if successful
 */
bool EditOperator::read(int id, EditorCache *list)
{
    if(id < 0 || editorCache->count() <= id){
        return false;
    }

    //copy cache
    *list = editorCache->at(id);

    return true;
}

/**
 * @fn EditOperator::readAll
 * @brief Get cache data pointer
 * @param list Pointer to store the result
 */
void EditOperator::readAll(QList<EditorCache> *list)
{
    *list = editorCache->getCacheCopy();
}

/**
 * @fn EditOperator::addAction
 * @brief Add new index.
 *        it works only exec, filesearch, plugin, profileload
 */
void EditOperator::addAction()
{
    //need replace
    AddCommand *com = new AddCommand(editorCache->count(), editorCache);
    undostack->push(com);
}

/**
 * @fn EditOperator::insertAction
 * @brief Insert new index
 * @param index Insert index
 * @param editorcache Insert data
 */
void EditOperator::insertAction(int index, EditorCache *editorcache)
{
    InsertCommand *com = new InsertCommand(index, *editorcache, editorCache);
    undostack->push(com);
}

/**
 * @fn EditOperator::deleteAction
 * @brief Delete specified XML index.
 * @param id Row number to read, starting from 0
 */
void EditOperator::deleteAction(int id)
{
    DeleteCommand *com = new DeleteCommand(id, editorCache->at(id), editorCache);
    undostack->push(com);
}




/**
 * @fn EditOperator::editTabAction
 * @brief Called when the function tab is changed
 * @param id       Row number to read, starting from 0
 * @param newindex New tab position
 */
void EditOperator::editTabAction(int id, int newindex)
{
    EditTabCommand *com = new EditTabCommand(id, newindex, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::comboboxLocalValAction
 * @brief Called when changing the combo box of the file search return variable
 * @param id    Row number to read, starting from 0
 * @param mnew  Changed variable name
 */
void EditOperator::comboboxLocalValAction(int id, QString mnew)
{
    EditComboBoxCommand *com = new EditComboBoxCommand(id, mnew, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @fn EditOperator::comboboxSearchAction
 * @brief Called when the file search setting name is changed
 * @param id     Row number to read, starting from 0
 * @param newstr New setting name
 * @param newval New setting combobox index
 */
void EditOperator::comboboxSearchAction(int id, QString newstr, int newval)
{
    //search combobox only
    EditScomboBoxCommand *com = new EditScomboBoxCommand(id, newstr, newval, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::comboboxPluginAction
 * @brief The function of the plug-in selection combo box on the Plugin tab
 * @param id      Row number to read, starting from 0
 * @param newstr  New setting name
 * @param newfile New setting plugin file path
 */
void EditOperator::comboboxPluginAction(int id, QString newstr, QString newfile)
{
    ComboPluginSelect *com = new ComboPluginSelect(id, newstr, newfile, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::comboboxProfileAction
 * @brief The function of the profile selection combo box on the ProfileLoad tab
 * @param id      Row number to read, starting from 0
 * @param newstr  New selected profile name
 * @param newfile New selected profile file path
 */
void EditOperator::comboboxProfileAction(int id, QString newstr, QString newfile)
{
    ComboProfileSelect *com = new ComboProfileSelect(id, newstr, newfile, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}





/**
 * @fn EditOperator::spinTimeoutAction
 * @brief The function of timeout value in spin box on the Execute tab
 * @param id       Row number to read, starting from 0
 * @param newvalue Timeout time in ms
 */
void EditOperator::spinTimeoutAction(int id, int newvalue)
{
    EditTimeout *com = new EditTimeout(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::spinLoopMaxAction
 * @brief The function to set the maximum number of input file processing
 * @param id       Row number to read, starting from 0
 * @param newvalue Maximum number of file inputs
 */
void EditOperator::spinLoopMaxAction(int id, int newvalue)
{
    EditLoopMax *com = new EditLoopMax(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::spinLoopArgumentsAction
 * @brief The function of setting arguments in one process loop in spin box on the Information tab
 * @param id       Row number to read, starting from 0
 * @param newvalue Number of arguments used in one process loop
 */
void EditOperator::spinLoopArgumentsAction(int id, int newvalue)
{
    EditLoopArguments *com = new EditLoopArguments(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::spinLoopRecursiveAction
 * @brief The function of maximum recursive count value in the spin box on the Information tab
 * @param id       Row number to read, starting from 0
 * @param newvalue Maximum count in a recursive loop
 */
void EditOperator::spinLoopRecursiveAction(int id, int newvalue)
{
    EditLoopRecursive *com = new EditLoopRecursive(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::radioSearchOutputAction
 * @brief The function that determines the output destination of search results on the FileSearch tab
 * @param id       Row number to read, starting from 0
 * @param newvalue Variant : 0, External file : 1
 */
void EditOperator::radioSearchOutputAction(int id, int newvalue)
{
    EditSearchOutput *com = new EditSearchOutput(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::radioSearchFileOutputTypeAction
 * @brief The function that determines the writing method
 *        when outputting an external file on the FileSearch tab
 * @param id       Row number to read, starting from 0
 * @param newvalue Append : 0, Overwrite : 1
 */
void EditOperator::radioSearchFileOutputTypeAction(int id, int newvalue)
{
    EditSearchFileOutputType *com = new EditSearchFileOutputType(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @fn EditOperator::tableSwapExecAction
 * @brief Swap arguments in the table on the Execute tab
 * @param id       Row number to read, starting from 0
 * @param beforeid The row of the source table in the argument list of selected id
 * @param afterid  The row of the destination table in the argument list of the selected id
 */
void EditOperator::tableSwapExecAction(int id, int beforeid, int afterid)
{
    SwapExecTable *com = new SwapExecTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::tableSwapPluginAction
 * @brief Swap arguments in the table on the Plugin tab
 * @param id       Row number to read, starting from 0
 * @param beforeid The row of the source table in the argument list of selected id
 * @param afterid  The row of the destination table in the argument list of the selected id
 */
void EditOperator::tableSwapPluginAction(int id, int beforeid, int afterid)
{
    SwapPluginTable *com = new SwapPluginTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @deprecated
 * @fn EditOperator::tableEditVariantAction
 * @brief The function to edit the argument at the specified position in the table on the Variant tree
 * @param id        Row number to read, starting from 0
 * @param tableid   The line number of the edited argument table in the Variant tree
 * @param variants  Variant and value pair
 * @param operation Operation defined in UiCommandMap
 */
void EditOperator::tableEditVariantAction(int id, int tableid, QStringList variants, int operation)
{
    EditLocalVarTable *com = new EditLocalVarTable(id, tableid, variants.at(0), variants.at(1), operation, editorCache);

    undostack->push(com);
//    emit editUpdate(id);? no need to update
}

/**
 * @fn EditOperator::tableEditExecAction
 * @brief The function to edit the argument at the specified position in the table on the Execute tab
 * @param id        Row number to read, starting from 0
 * @param tableid   The line number of the edited argument table in the Execute tab
 * @param newstr    Argument value
 * @param operation Operation defined in UiCommandMap
 */
void EditOperator::tableEditExecAction(int id, int tableid, QString newstr, int operation)
{
     EditExecTable *com = new EditExecTable(id, tableid, newstr, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::tableEditPluginAction
 * @brief The function to edit the argument at the specified position in the table on the Plugin tab
 * @param id        Row number to read, starting from 0
 * @param tableid   The line number of the edited argument table in the Plugin tab
 * @param newstr    Argument value
 * @param operation Operation defined in UiCommandMap
 */
void EditOperator::tableEditPluginAction(int id, int tableid, QString newstr, int operation)
{
    EditPluginTable *com = new EditPluginTable(id, tableid, newstr, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::tableEditPluginAction
 * @brief The function to update all argument list tables in plugin tab
 * @param id         Row number to read, starting from 0
 * @param newstrlist Argument list edited on the Plugins tab
 * @param operation  Operation defined in UiCommandMap
 */
void EditOperator::tableEditPluginAction(int id, QStringList newstrlist, int operation)
{
    EditPluginTable *com = new EditPluginTable(id, newstrlist, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::tableDragDropExecAction
 * @brief The function when dragging and dropping in the table on the Execute tab
 * @param id       Row number to read, starting from 0
 * @param beforeid The row of the source table in the argument list of selected id
 * @param afterid  The row of the destination table in the argument list of the selected id
 */
void EditOperator::tableDragDropExecAction(int id, QList<int> beforeid, int afterid)
{
    DragDropExecTable *com = new DragDropExecTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::tableDragDropPluginAction
 * @brief The function when dragging and dropping in the table on the Plugin tab
 * @param id       Row number to read, starting from 0
 * @param beforeid The row of the source table in the argument list of selected id
 * @param afterid  The row of the destination table in the argument list of the selected id
 */
void EditOperator::tableDragDropPluginAction(int id, QList<int> beforeid, int afterid)
{
    DragDropPluginTable *com = new DragDropPluginTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::treeEditGVariantAction
 * @brief The function called when the Global value is edited on the Variant tree
 * @param id Row number to read, starting from 0
 * @param variants The list containing variable and value
 * @param operation Operation defined in UiCommandMap
 */
void EditOperator::treeEditGVariantAction(int id, QStringList variants, int operation)
{
    EditGlobalVarTree *com = new EditGlobalVarTree(id, variants, operation);
    undostack->push(com);
}

/**
 * @fn EditOperator::treeSwapGVariantAction
 * @brief Swap positions of variant and value pair in the table on the Variant tree
 * @param beforeid The row of the source table in the argument list
 * @param afterid  The row of the destination table in the argument list
 */
void EditOperator::treeSwapGVariantAction(int beforeid, int afterid)
{
    SwapGlobalVarTree *com = new SwapGlobalVarTree(beforeid, afterid);
    undostack->push(com);
}

/**
 * @fn EditOperator::treeEditLVariantAction
 * @brief The function called when the Local value is edited on the Variant tree
 * @param id         Row number to read, starting from 0
 * @param variants   The list containing variable and value
 * @param operation  Operation defined in UiCommandMap
 * @param localindex Row number in Local variant (Current index: 1)
 */
void EditOperator::treeEditLVariantAction(int id, QStringList variants, int operation, int localindex)
{
    EditLocalVariantTree *com = new EditLocalVariantTree(localindex, id, variants.at(0), variants.at(1), operation, editorCache);
    undostack->push(com);
}

/**
 * @fn EditOperator::treeSwapLVariantAction
 * @brief Swap positions of variant and value pair in the table on the Variant tree
 * @param beforeid   The row of the source table in the argument list of selected id
 * @param afterid    The row of the destination table in the argument list of the selected id
 * @param localindex Row number in Local variant (Current index: 1)
 */
void EditOperator::treeSwapLVariantAction(int beforeid, int afterid, int localindex)
{
    SwapLocalVariantTree *com = new SwapLocalVariantTree(localindex, beforeid, afterid, editorCache);
    undostack->push(com);
}





/**
 * @fn EditOperator::textFileOutputAction
 * @brief The output file path in QLineEdit on FileSearch tab
 * @param id Row number to read, starting from 0
 * @param newstr Output file path
 */
void EditOperator::textFileOutputAction(int id, QString newstr)
{
    StringFileOutput *com = new StringFileOutput(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textProjectAuthorAction
 * @brief Author name in QLineEdit on the Information tab
 * @param id     Row number to read, starting from 0
 * @param newstr Author name
 */
void EditOperator::textProjectAuthorAction(int id, QString newstr)
{
    StringProjectAuthor *com = new StringProjectAuthor(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textProjectDescriptAction
 * @brief Project description in QLineEdit on the Information tab
 * @param id     Row number to read, starting from 0
 * @param newstr Project description
 */
void EditOperator::textProjectDescriptAction(int id, QString newstr)
{
    StringProjectDescription *com = new StringProjectDescription(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textProjectNameAction
 * @brief Project name in QLineEdit on the Information tab
 * @param id     Row number to read, starting from 0
 * @param newstr Project name
 */
void EditOperator::textProjectNameAction(int id, QString newstr)
{
    StringProjectName *com = new StringProjectName(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textProjectVerAction
 * @brief Project version in QLineEdit on the Information tab
 * @param id     Row number to read, starting from 0
 * @param newstr Project version
 */
void EditOperator::textProjectVerAction(int id, QString newstr)
{
    StringProjectVersion *com = new StringProjectVersion(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textSearchSeparateAction
 * @brief The function of QLineEdit to enter the character
 *        that separates the search result file name of FileSearch tab
 * @param id     Row number to read, starting from 0
 * @param newstr New separator text
 */
void EditOperator::textSearchSeparateAction(int id, QString newstr)
{
    StringSearchSep *com = new StringSearchSep(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::textBasePathAction
 * @brief The function of QLineEdit to set the profile launch path
 *        on the Information tab.
 * @param id     Row number to read, starting from 0
 * @param newstr New basepath text
 */
void EditOperator::textBasePathAction(int id, QString newstr)
{
    StringBasePath *com = new StringBasePath(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkAllowInputAction
 * @brief The function to allow or disallow input files to profile
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkAllowInputAction(int id, bool newcheck)
{
    CheckEditAllowInput *com = new CheckEditAllowInput(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkLoopInfAction
 * @brief The function of the checkbox to decide whether to process all input files
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkLoopInfAction(int id, bool newcheck)
{
    CheckEditLoopInf *com = new CheckEditLoopInf(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkSearchInputAction
 * @brief Checkbox for the function that substitutes the input file
 *        list at profile startup from the output of the search profile
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkSearchInputAction(int id, bool newcheck)
{
    CheckEditSearchInput *com = new CheckEditSearchInput(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkTimeoutAction
 * @brief The function to enable timeout when an application does not work
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkTimeoutAction(int id, bool newcheck)
{
    CheckEditTimeout *com = new CheckEditTimeout(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkRunDetachAction
 * @brief The function to decide whether to execute the next process
 *        without waiting for the end after starting the application
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkRunDetachAction(int id, bool newcheck)
{
    CheckEditRunDetach *com = new CheckEditRunDetach(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkOnlyNormalAction
 * @brief The function to select whether to execute only when executed by AutoBatchScheduler
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkOnlyNormalAction(int id, bool newcheck)
{
    CheckOnlySchedulerNormal *com = new CheckOnlySchedulerNormal(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkOnlySearchAction
 * @brief The function to select whether to execute only when executed by AutoBatchScheduler
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkOnlySearchAction(int id, bool newcheck)
{
    CheckOnlySchedulerSearch *com = new CheckOnlySchedulerSearch(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkOnlyPluginAction
 * @brief The function to select whether to execute only when executed by AutoBatchScheduler
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkOnlyPluginAction(int id, bool newcheck)
{
    CheckOnlySchedulerPlugin *com = new CheckOnlySchedulerPlugin(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::checkOnlyOtherAction
 * @brief The function to select whether to execute only when executed by AutoBatchScheduler
 * @param id       Row number to read, starting from 0
 * @param newcheck True if enabled
 */
void EditOperator::checkOnlyOtherAction(int id, bool newcheck)
{
    CheckOnlySchedulerOther *com = new CheckOnlySchedulerOther(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @brief EditOperator::cutAction
 * @param id Row number to read, starting from 0
 */
void EditOperator::cutAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setListData(editorCache->at(id));
    clipboard->setMimeData(lmime);

    deleteAction(id);
}

void EditOperator::copyAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setListData(editorCache->at(id));
    clipboard->setMimeData(lmime);
}

void EditOperator::pasteAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    const ListMimeData *lmime = qobject_cast<const ListMimeData *>(clipboard->mimeData());
    EditorCache clipdata = ListMimeData::getListData(lmime);

    insertAction(id, &clipdata);
}

/**
 * @fn EditOperator::swapAction
 * @brief Process table swap operation
 * @param before The row of the source table in the argument list of selected index
 * @param after  The row of the destination table in the argument list of the selected index
 */
void EditOperator::swapAction(int before, int after)
{
    //index regulation
    int ccount = editorCache->count() - 1;
    if(before == after
            || before < XML_MIN
            || after < XML_MIN
            || before > ccount
            || after > ccount)
        return;

    SwapCommand *com = new SwapCommand(before, after, editorCache);
    undostack->push(com);
}

void EditOperator::dragDropAction(QList<int> before, int after)
{
    DragDropCommand *com = new DragDropCommand(before, after, editorCache);
    undostack->push(com);
}

//bool EditOperator::elementDiffCheck()
//{

//    return false;
//}

void EditOperator::newAction()
{
#ifdef QT_DEBUG
    qDebug() << "[EditOperator::newAction]";
#endif

    reset();

    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    autosavefile = settings.value(sc.ABE_AUTOSAVE_DIR, "./").toString();

    autosavefile.append(QString(".~$new_") \
                        .append(QString::number(QDateTime::currentSecsSinceEpoch())) \
                        .append(".autosave"));

    //create xmlfile
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(autosavefile);
    delete updater;

    //temp loadfile change
    loadfile = autosavefile;
    loadcache(0);

    emit loadfileChanged(autosavefile);
}

/**
 * @fn EditOperator::autoSaveRecoveryAction
 * @brief Load a *.autosave file that remains when the application crashes unexpectedly.
 * @param filepath *.autosave file
 */
void EditOperator::autoSaveRecoveryAction(QString filepath)
{
    autosavefile = filepath;

    //create xmlfile
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(autosavefile);
    delete updater;

    //temp loadfile change
    loadfile = autosavefile;
    loadcache(0);

    emit loadfileChanged(autosavefile);
}

void EditOperator::openAction(QString filepath)
{
    reset();

    //generate copy file
    QFileInfo info(filepath);

    //file is not exist then return
    if(!info.exists()) return;

    autosavefile = "./.~$" + info.fileName();
    loadfile = filepath;

    //copy file
    QFile::copy(filepath, autosavefile);

    //load action TODO:init loading num
    loadcache(0);

    // save recent files list (max: 10)
    QSettings settings( sc.OUTPUT_FILE, QSettings::IniFormat );
    QStringList list = settings.value(sc.ABE_RECENT_FILES).value<QStringList>();
    int recentcount = list.count();

    list.removeOne(loadfile);
    list.insert(0, loadfile);

    if(recentcount > 10) list.removeLast();
    settings.setValue(sc.ABE_RECENT_FILES, list);

    emit loadfileChanged(loadfile);
}

void EditOperator::saveAction(QString filepath)
{
    //force save last changes
    save();

    //delete existed files
    if(QFile::exists(filepath)){
        QFile::remove(filepath);
    }

    //copy to dest file
    QFile::copy(autosavefile, filepath);

    loadfile = filepath;

    emit loadfileChanged(loadfile);
}

void EditOperator::exportAction(QString filepath)
{
    //force save last changes
    save();

    //delete existed files
    if(QFile::exists(filepath)){
        QFile::remove(filepath);
    }

    //fetch all cache data
    //full update file
    QList<QList<QStringList> *> *tempcache = new QList<QList<QStringList> *>;
    ProcessXmlListGenerator *xgen = new ProcessXmlListGenerator();

    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(autosavefile);
    updater->readAllItem(tempcache);

    //optimize xml structure
    int count = tempcache->count();

    //avoid info, local list
    for(int i = XML_MIN; i < count; i++){
        xgen->createSeparateList(tempcache->at(i));
    }

    //full update file
    ProcessXmlBuilder *mexport = new ProcessXmlBuilder();
    mexport->setLoadBlankPath(filepath);    
    mexport->writeAllItem(tempcache);

    delete updater;
    delete mexport;
    delete xgen;
    delete tempcache;

}

void EditOperator::abortAction()
{
    reset();
}

void EditOperator::save()
{
    //overwrite all items
    if(QFile::exists(autosavefile)){
        //condition of currently file used
        QFile file(autosavefile);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            file.close();
            QFile::remove(autosavefile);
        }else{
            return;
        }
    }

    QList<QList<QStringList> *> dest;
    editorCache->serialize(&dest);

    //full update file
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadBlankPath(autosavefile);

#ifdef QT_DEBUG
    qDebug() << "[EditOperator::save] update Timer start";
    QElapsedTimer time;
    time.start();
#endif

    updater->writeAllItem(&dest);

#ifdef QT_DEBUG
    qDebug() << "[EditOperator::save] elapsed: " << time.elapsed() << "ms";
#endif

    delete updater;
}

/**
 * @fn EditOperator::loadcache
 * @brief Read apro, xml and create all list.
 * @param amount currently unused
 * @todo Considered page loading to speed up the display, but failed.
 */
void EditOperator::loadcache(int amount)
{
    Q_UNUSED(amount)
    if(loadfile == "") return;

    //append new index along with "amount" number.
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(loadfile);

#ifdef QT_DEBUG
    qDebug() << "[EditOperator::loadcache] Timer start";
    QElapsedTimer time;
    time.start();
#endif

    QList<QList<QStringList> *> cache;
    updater->readAllItem(&cache);
    editorCache->deSerialize(&cache);

#ifdef QT_DEBUG
    qDebug() << "[EditOperator::loadcache] elapsed: " << time.elapsed() << "ms";
#endif

    delete updater;
}

void EditOperator::reset()
{
    //delete existed files
    if(QFile::exists(autosavefile)){
        QFile::remove(autosavefile);
    }

    autosavefile = "";
    loadfile = "";

    //delete all "new cache"
    editorCache->clear();

    //reset undo action
    undostack->clear();
}

QString EditOperator::getLoadfile() const
{
    return loadfile;
}

int EditOperator::getCacheSize() const
{
    return editorCache->count();
}

int EditOperator::getCurrentCommandType()
{
    qDebug() << "[EditOperator::getCurrentCommandType] stack : " << undostack->index();
    return undostack->command(undostack->index())->id();
}

QRect EditOperator::getMainWindowGeometry()
{
    return parentwid->geometry();
}

QUndoStack *EditOperator::getUndostack() const
{
    return undostack;
}

void EditOperator::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerid){
        //autosave commands
        qDebug() << "[EditOperator::timerEvent] save()";
        save();
    }
}
