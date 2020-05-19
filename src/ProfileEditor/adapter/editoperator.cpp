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
//    undoStack2 = new QUndoStack();
//    undostack->setUndoLimit(0);

//    cache = new QList<QList<QStringList>*>();
    editorCache = new EditorCacheList();

    parentwid = qobject_cast<QMainWindow *>(parent);

    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //init timer (ms)
    settings.beginGroup("pe_general");
    timerid = startTimer(settings.value("profileeditor/autosaveperiod", 1).toInt() * 60000);
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
//    delete cache;
}

//bool EditOperator::read(int id, QList<QStringList> *list)
//{
//    if(id < 0 || cache->count() <= id){
//        list->clear();
//        return false;
//    }

//    //copy cache
//    QMutableListIterator<QStringList> i(*cache->at(id));
//    while(i.hasNext()){
//        list->append(i.next());
//    }

//    return true;
//}

/**
 * @brief EditOperator::read
 * @param id
 * @param list
 * @return
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

//void EditOperator::readAll(QList<QList<QStringList> *> *list)
//{
//    *list = *cache;
//}

/**
 * @fn EditOperator::readAll
 * @brief Get cache data pointer
 * @param list Pointer of EditorCacheList
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
    //@deprecated
//    {
//    //create xml inner
//    QList<QStringList> xmlstruct;
//    ProcessXmlListGenerator *xgen = new ProcessXmlListGenerator();
//    xgen->createNewList(&xmlstruct);
//    delete xgen;

//    AddCommand *com = new AddCommand(cache->count(), &xmlstruct, cache);
//    //undostack->push(com);
//    }

    //need replace
    AddCommand *com = new AddCommand(editorCache->count(), editorCache);
    undostack->push(com);
}

//void EditOperator::insertAction(int id, QList<QStringList> *xmlstruct)
//{
//    InsertCommand *com = new InsertCommand(id,xmlstruct,cache);
//    undostack->push(com);
//}

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
 * @param id Deleted XML index
 */
void EditOperator::deleteAction(int id)
{
    // @deprecated
//    {
//    DeleteCommand *com = new DeleteCommand(id,cache->at(id), cache);
//    }

    DeleteCommand *com = new DeleteCommand(id, editorCache->at(id), editorCache);
    undostack->push(com);

}




/**
 * @fn EditOperator::editTabAction
 * @brief Called when the function tab is changed
 * @param id       Cache data id
 * @param newindex New tab position
 */
void EditOperator::editTabAction(int id, int newindex)
{
//    EditTabCommand *com = new EditTabCommand(id, newindex, cache);
    EditTabCommand *com = new EditTabCommand(id, newindex, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @fn EditOperator::comboboxLocalValAction
 * @brief Called when changing the combo box of the file search return variable
 * @param id    Cache data id
 * @param mnew  Changed variable name
 */
void EditOperator::comboboxLocalValAction(int id, QString mnew)
{
    //localvariant combobox only
//    EditComboBoxCommand *com = new EditComboBoxCommand(id, mnew, cache);
    EditComboBoxCommand *com = new EditComboBoxCommand(id, mnew, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @fn EditOperator::comboboxSearchAction
 * @brief Called when the file search setting name is changed
 * @param id     Cache data id
 * @param newstr New setting name
 * @param newval New setting combobox index
 */
void EditOperator::comboboxSearchAction(int id, QString newstr, int newval)
{
    //search combobox only
//    EditScomboBoxCommand *com = new EditScomboBoxCommand(id, newstr, newval, cache);
    EditScomboBoxCommand *com = new EditScomboBoxCommand(id, newstr, newval, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::comboboxPluginAction
 * @param id      Cache data id
 * @param newstr  New setting name
 * @param newfile New setting plugin file path
 */
void EditOperator::comboboxPluginAction(int id, QString newstr, QString newfile)
{
//    ComboPluginSelect *com = new ComboPluginSelect(id, newstr, newfile, cache);
    ComboPluginSelect *com = new ComboPluginSelect(id, newstr, newfile, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::comboboxProfileAction
 * @param id
 * @param newstr
 * @param newfile
 */
void EditOperator::comboboxProfileAction(int id, QString newstr, QString newfile)
{
//    ComboProfileSelect *com = new ComboProfileSelect(id, newstr, newfile, cache);
    ComboProfileSelect *com = new ComboProfileSelect(id, newstr, newfile, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}





/**
 * @brief EditOperator::spinTimeoutAction
 * @param id
 * @param newvalue
 */
void EditOperator::spinTimeoutAction(int id, int newvalue)
{
//    EditTimeout *com = new EditTimeout(id, newvalue, cache);
    EditTimeout *com = new EditTimeout(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::spinLoopMaxAction
 * @param id
 * @param newvalue
 */
void EditOperator::spinLoopMaxAction(int id, int newvalue)
{
//    EditLoopMax *com = new EditLoopMax(id, newvalue, cache);
    EditLoopMax *com = new EditLoopMax(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::spinLoopArgumentsAction
 * @param id
 * @param newvalue
 */
void EditOperator::spinLoopArgumentsAction(int id, int newvalue)
{
//    EditLoopArguments *com = new EditLoopArguments(id, newvalue, cache);
    EditLoopArguments *com = new EditLoopArguments(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::spinLoopRecursiveAction
 * @param id
 * @param newvalue
 */
void EditOperator::spinLoopRecursiveAction(int id, int newvalue)
{
//    EditLoopRecursive *com = new EditLoopRecursive(id, newvalue, cache);
    EditLoopRecursive *com = new EditLoopRecursive(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::radioSearchOutputAction
 * @param id
 * @param newvalue
 */
void EditOperator::radioSearchOutputAction(int id, int newvalue)
{
//    EditSearchOutput *com = new EditSearchOutput(id, newvalue, cache);

    EditSearchOutput *com = new EditSearchOutput(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::radioSearchFileOutputTypeAction
 * @param id
 * @param newvalue
 */
void EditOperator::radioSearchFileOutputTypeAction(int id, int newvalue)
{
//    EditSearchFileOutputType *com = new EditSearchFileOutputType(id, newvalue, cache);
    EditSearchFileOutputType *com = new EditSearchFileOutputType(id, newvalue, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @brief EditOperator::tableSwapExecAction
 * @param id
 * @param beforeid
 * @param afterid
 */
void EditOperator::tableSwapExecAction(int id, int beforeid, int afterid)
{
//    SwapExecTable *com = new SwapExecTable(id, beforeid, afterid, cache);
    SwapExecTable *com = new SwapExecTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::tableSwapPluginAction
 * @param id
 * @param beforeid
 * @param afterid
 */
void EditOperator::tableSwapPluginAction(int id, int beforeid, int afterid)
{
//    SwapPluginTable *com = new SwapPluginTable(id, beforeid, afterid, cache);
    SwapPluginTable *com = new SwapPluginTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @brief EditOperator::tableEditVariantAction
 * @param id
 * @param tableid
 * @param variants
 * @param operation
 */
void EditOperator::tableEditVariantAction(int id, int tableid, QStringList variants, int operation)
{
//    EditLocalVarTable *com = new EditLocalVarTable(id, tableid, variants, operation, cache);
    EditLocalVarTable *com = new EditLocalVarTable(id, tableid, variants.at(0), variants.at(1), operation, editorCache);

    undostack->push(com);
//    emit editUpdate(id);? no need to update
}

/**
 * @brief EditOperator::tableEditExecAction
 * @param id
 * @param tableid
 * @param newstr
 * @param operation
 */
void EditOperator::tableEditExecAction(int id, int tableid, QString newstr, int operation)
{
//    EditExecTable *com = new EditExecTable(id, tableid, newstr, operation, cache);
     EditExecTable *com = new EditExecTable(id, tableid, newstr, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::tableEditPluginAction
 * @param id
 * @param tableid
 * @param newstr
 * @param operation
 */
void EditOperator::tableEditPluginAction(int id, int tableid, QString newstr, int operation)
{
//    EditPluginTable *com = new EditPluginTable(id, tableid, newstr, operation, cache);
    EditPluginTable *com = new EditPluginTable(id, tableid, newstr, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::tableEditPluginAction
 * @param id
 * @param newstrlist
 * @param operation
 */
void EditOperator::tableEditPluginAction(int id, QStringList newstrlist, int operation)
{
//    EditPluginTable *com = new EditPluginTable(id, newstrlist, operation, cache);
    EditPluginTable *com = new EditPluginTable(id, newstrlist, operation, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::tableDragDropExecAction
 * @param id
 * @param beforeid
 * @param afterid
 */
void EditOperator::tableDragDropExecAction(int id, QList<int> beforeid, int afterid)
{
//    DragDropExecTable *com = new DragDropExecTable(id, beforeid, afterid, cache);
    DragDropExecTable *com = new DragDropExecTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::tableDragDropPluginAction
 * @param id
 * @param beforeid
 * @param afterid
 */
void EditOperator::tableDragDropPluginAction(int id, QList<int> beforeid, int afterid)
{
//    DragDropPluginTable *com = new DragDropPluginTable(id, beforeid, afterid, cache);
    DragDropPluginTable *com = new DragDropPluginTable(id, beforeid, afterid, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::treeEditGVariantAction
 * @param id
 * @param variants
 * @param operation
 */
void EditOperator::treeEditGVariantAction(int id, QStringList variants, int operation)
{
    EditGlobalVarTree *com = new EditGlobalVarTree(id, variants, operation);
    undostack->push(com);
}

/**
 * @brief EditOperator::treeSwapGVariantAction
 * @param beforeid
 * @param afterid
 */
void EditOperator::treeSwapGVariantAction(int beforeid, int afterid)
{
    SwapGlobalVarTree *com = new SwapGlobalVarTree(beforeid, afterid);
    undostack->push(com);
}

/**
 * @brief EditOperator::treeEditLVariantAction
 * @param id
 * @param variants
 * @param operation
 * @param localindex
 */
void EditOperator::treeEditLVariantAction(int id, QStringList variants, int operation, int localindex)
{
//    EditLocalVariantTree *com = new EditLocalVariantTree(localindex, id, variants, operation, cache);
    EditLocalVariantTree *com = new EditLocalVariantTree(localindex, id, variants.at(0), variants.at(1), operation, editorCache);
    undostack->push(com);
}

/**
 * @brief EditOperator::treeSwapLVariantAction
 * @param beforeid
 * @param afterid
 * @param localindex
 */
void EditOperator::treeSwapLVariantAction(int beforeid, int afterid, int localindex)
{
//    SwapLocalVariantTree *com = new SwapLocalVariantTree(localindex, beforeid, afterid, cache);
    SwapLocalVariantTree *com = new SwapLocalVariantTree(localindex, beforeid, afterid, editorCache);
    undostack->push(com);
}





/**
 * @brief EditOperator::textFileOutputAction
 * @param id
 * @param newstr
 */
void EditOperator::textFileOutputAction(int id, QString newstr)
{
//    StringFileOutput *com = new StringFileOutput(id, newstr, cache);
    StringFileOutput *com = new StringFileOutput(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::textProjectAuthorAction
 * @param id
 * @param newstr
 */
void EditOperator::textProjectAuthorAction(int id, QString newstr)
{
//    StringProjectAuthor *com = new StringProjectAuthor(id, newstr, cache);
    StringProjectAuthor *com = new StringProjectAuthor(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::textProjectDescriptAction
 * @param id
 * @param newstr
 */
void EditOperator::textProjectDescriptAction(int id, QString newstr)
{
//    StringProjectDescription *com = new StringProjectDescription(id, newstr, cache);
    StringProjectDescription *com = new StringProjectDescription(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::textProjectNameAction
 * @param id
 * @param newstr
 */
void EditOperator::textProjectNameAction(int id, QString newstr)
{
//    StringProjectName *com = new StringProjectName(id, newstr, cache);
    StringProjectName *com = new StringProjectName(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::textProjectVerAction
 * @param id
 * @param newstr
 */
void EditOperator::textProjectVerAction(int id, QString newstr)
{
//    StringProjectVersion *com = new StringProjectVersion(id, newstr, cache);
    StringProjectVersion *com = new StringProjectVersion(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::textSearchSeparateAction
 * @param id
 * @param newstr
 */
void EditOperator::textSearchSeparateAction(int id, QString newstr)
{
//    StringSearchSep *com = new StringSearchSep(id, newstr, cache);
    StringSearchSep *com = new StringSearchSep(id, newstr, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @brief EditOperator::checkAllowInputAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkAllowInputAction(int id, bool newcheck)
{
//    CheckEditAllowInput *com = new CheckEditAllowInput(id, newcheck, cache);
    CheckEditAllowInput *com = new CheckEditAllowInput(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkLoopInfAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkLoopInfAction(int id, bool newcheck)
{
//    CheckEditLoopInf *com = new CheckEditLoopInf(id, newcheck, cache);
    CheckEditLoopInf *com = new CheckEditLoopInf(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkSearchInputAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkSearchInputAction(int id, bool newcheck)
{
//    CheckEditSearchInput *com = new CheckEditSearchInput(id, newcheck, cache);
    CheckEditSearchInput *com = new CheckEditSearchInput(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkTimeoutAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkTimeoutAction(int id, bool newcheck)
{
//    CheckEditTimeout *com = new CheckEditTimeout(id, newcheck, cache);
    CheckEditTimeout *com = new CheckEditTimeout(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkRunDetachAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkRunDetachAction(int id, bool newcheck)
{
//    CheckEditRunDetach *com = new CheckEditRunDetach(id, newcheck, cache);
    CheckEditRunDetach *com = new CheckEditRunDetach(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkOnlyNormalAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkOnlyNormalAction(int id, bool newcheck)
{
//    CheckOnlySchedulerNormal *com = new CheckOnlySchedulerNormal(id, newcheck, cache);
    CheckOnlySchedulerNormal *com = new CheckOnlySchedulerNormal(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkOnlySearchAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkOnlySearchAction(int id, bool newcheck)
{
//    CheckOnlySchedulerSearch *com = new CheckOnlySchedulerSearch(id, newcheck, cache);
    CheckOnlySchedulerSearch *com = new CheckOnlySchedulerSearch(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkOnlyPluginAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkOnlyPluginAction(int id, bool newcheck)
{
//    CheckOnlySchedulerPlugin *com = new CheckOnlySchedulerPlugin(id, newcheck, cache);
    CheckOnlySchedulerPlugin *com = new CheckOnlySchedulerPlugin(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}

/**
 * @brief EditOperator::checkOnlyOtherAction
 * @param id
 * @param newcheck
 */
void EditOperator::checkOnlyOtherAction(int id, bool newcheck)
{
//    CheckOnlySchedulerOther *com = new CheckOnlySchedulerOther(id, newcheck, cache);
    CheckOnlySchedulerOther *com = new CheckOnlySchedulerOther(id, newcheck, editorCache);

    undostack->push(com);
    emit editUpdate(id);
}




/**
 * @brief EditOperator::cutAction
 * @param id
 */
void EditOperator::cutAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

//    lmime->setListData(cache->at(id));
    lmime->setListData(editorCache->at(id));
    clipboard->setMimeData(lmime);

    deleteAction(id);
//    DeleteCommand *com = new DeleteCommand(id, cache->at(id), cache);
//    undostack->push(com);

}

void EditOperator::copyAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

//    lmime->setListData(cache->at(id));
    lmime->setListData(editorCache->at(id));
    clipboard->setMimeData(lmime);
}

void EditOperator::pasteAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    const ListMimeData *lmime = qobject_cast<const ListMimeData *>(clipboard->mimeData());
//    QList<QStringList> tlist(ListMimeData::getListData(lmime));
    EditorCache clipdata = ListMimeData::getListData(lmime);

//    insertAction(id, &tlist);
    insertAction(id, &clipdata);
}

/**
 * @brief EditOperator::swapAction
 * @param before
 * @param after
 */
void EditOperator::swapAction(int before, int after)
{
    //index regulation
//    int ccount = cache->count() - 1;
    int ccount = editorCache->count() - 1;
    if(before == after
            || before < XML_MIN
            || after < XML_MIN
            || before > ccount
            || after > ccount)
        return;

//    SwapCommand *com = new SwapCommand(before, after, cache);
    SwapCommand *com = new SwapCommand(before, after, editorCache);
    undostack->push(com);
}

void EditOperator::dragDropAction(QList<int> before, int after)
{
//    DragDropCommand *com = new DragDropCommand(before, after, cache);
    DragDropCommand *com = new DragDropCommand(before, after, editorCache);
    undostack->push(com);
}

bool EditOperator::elementDiffCheck()
{

    return false;
}

void EditOperator::newAction()
{
#ifdef QT_DEBUG
    qDebug() << endl << "[EditOperator::newAction]";
#endif

    reset();

    QSettings settings( "./settings.ini", QSettings::IniFormat );
    autosavefile = settings.value("profileeditor/tempdir", "./").toString();

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
    QSettings settings( "./settings.ini", QSettings::IniFormat );
    QStringList list = settings.value("profileeditor/recentfiles").value<QStringList>();
    int recentcount = list.count();

    list.removeOne(loadfile);
    list.insert(0, loadfile);

    if(recentcount > 10) list.removeLast();
    settings.setValue("profileeditor/recentfiles", list);

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
//    if(QFile::exists("test.xml")){
//        //condition of currently file used
//        QFile file("test.xml");
//        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
//            file.close();
//            QFile::remove("test.xml");
//        }else{
//            return;
//        }
//    }
//    ProcessXmlBuilder *updater3 = new ProcessXmlBuilder();
//    updater3->setLoadBlankPath("test.xml");
//    int ecount = dest.count();
//    for(int i = 0; i < ecount; i++){
//        updater3->addItem(dest.at(i));
//    }
//    delete updater3;

    //full update file
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadBlankPath(autosavefile);

#ifdef QT_DEBUG
    qDebug() << endl << "[EditOperator::save] update Timer start";
    QTime time;
    time.start();
#endif

//    int count = dest.count();
//    for(int i = 0; i < count; i++){
//        updater->insertItem(dest.at(i), i);
//    }
    updater->writeAllItem(&dest);

#ifdef QT_DEBUG
    qDebug() << "[EditOperator::save] elapsed: " << time.elapsed() << "ms";
#endif

//    if(loadfile == "") return;

    //append unload index.
//    ProcessXmlBuilder *updater2 = new ProcessXmlBuilder();
//    updater->setLoadPath(loadfile);

//    count = updater2->count();
//    QList<QStringList> list;
//    for(int i = loadedxmlid; i < count; i++){
//        list.clear();
//        updater2->readItem(i, &list);
//        updater->addItem(&list);
//    }

//    delete updater2;


//    loadcache(-1);

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

//    int count = updater->count();

//    //load all data (TODO:not efficient)
//    if(amount == 0) amount = count;
//    int i;
//    QList<QStringList> *list;
//    for(i = loadedxmlid; i < count; i++){
//        list = new QList<QStringList>();
//        if(i < amount){
//            updater->readItem(i, list);
//            cache->append(list);
//        }else{
//            break;
//        }
//    }

//    loadedxmlid = i - 1;
#ifdef QT_DEBUG
    qDebug() << endl << "[EditOperator::loadcache] Timer start";
    QTime time;
    time.start();
#endif

    QList<QList<QStringList> *> cache;
    updater->readAllItem(&cache);
//    loadedxmlid = updater->count() - 1;

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

//    loadedxmlid = 0;
    autosavefile = "";
    loadfile = "";

    //delete all "new cache"
//    qDeleteAll(*cache);
//    cache->clear();

    editorCache->clear();

    //reset undo action
    undostack->clear();
    //undoStack2->clear();

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

//void EditOperator::appendcache()
//{
//    if(loadfile == "") return;

//    //appending index
//    int amount = 50;

//    //append new index along with "amount" number.
//    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
//    updater->setLoadPath(loadfile);

//    int count = updater->count();
//    int i;
//    QList<QStringList> *list;
//    for(i = loadedxmlid; i < count; i++){
//        list = new QList<QStringList>();
//        if(i < (loadedxmlid + amount)){
//            updater->readItem(i, list);
//            cache->append(list);
//        }else{
//            break;
//        }
//    }

//    loadedxmlid = i - 1;

//    delete updater;
//}
