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
//    undostack->setUndoLimit(0);

    cache = new QList<QList<QStringList>*>();
    parentwid = qobject_cast<QMainWindow *>(parent);

    QSettings settings( "./settings.ini", QSettings::IniFormat );

    //init timer (ms)
    settings.beginGroup("pe_general");
    timerid = startTimer(settings.value("AUTOSAVEPERIOD", 5).toInt() * 60000);
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

    delete cache;
}

bool EditOperator::read(int id, QList<QStringList> *list)
{
    if(id < 0 || cache->count() <= id){
        list->clear();
        return false;
    }

    //copy cache
    QMutableListIterator<QStringList> i(*cache->at(id));
    while(i.hasNext()){
        list->append(i.next());
    }

    return true;
}

void EditOperator::readAll(QList<QList<QStringList> *> *list)
{
    *list = *cache;
}

void EditOperator::addAction()
{
    //create xml inner
    QList<QStringList> xmlstruct;
    ProcessXmlListGenerator *xgen = new ProcessXmlListGenerator();
    xgen->createNewList(&xmlstruct);
    delete xgen;

    AddCommand *com = new AddCommand(cache->count(), &xmlstruct, cache);
    undostack->push(com);

}

void EditOperator::insertAction(int id, QList<QStringList> *xmlstruct)
{
    InsertCommand *com = new InsertCommand(id,xmlstruct,cache);
    undostack->push(com);
}

/**
 * @fn EditOperator::deleteAction
 * @brief Delete specified XML index.
 * @param id Deleted XML index
 */
void EditOperator::deleteAction(int id)
{
    DeleteCommand *com = new DeleteCommand(id,cache->at(id), cache);
    undostack->push(com);

}





void EditOperator::editTabAction(int id, int newindex)
{
    EditTabCommand *com = new EditTabCommand(id, newindex, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::comboboxLocalValAction(int id, QString mnew)
{
    //localvariant combobox only
    EditComboBoxCommand *com = new EditComboBoxCommand(id, mnew, cache);

    undostack->push(com);
    emit editUpdate(id);
}





void EditOperator::comboboxSearchAction(int id, QString newstr, int newval)
{
    //search combobox only
    EditScomboBoxCommand *com = new EditScomboBoxCommand(id, newstr, newval, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::comboboxPluginAction(int id, QString newstr, QString newfile)
{
    ComboPluginSelect *com = new ComboPluginSelect(id, newstr, newfile, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::comboboxProfileAction(int id, QString newstr, QString newfile)
{
    ComboProfileSelect *com = new ComboProfileSelect(id, newstr, newfile, cache);

    undostack->push(com);
    emit editUpdate(id);
}






void EditOperator::spinTimeoutAction(int id, int newvalue)
{
    EditTimeout *com = new EditTimeout(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::spinLoopMaxAction(int id, int newvalue)
{
    EditLoopMax *com = new EditLoopMax(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::spinLoopArgumentsAction(int id, int newvalue)
{
    EditLoopArguments *com = new EditLoopArguments(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::spinLoopRecursiveAction(int id, int newvalue)
{
    EditLoopRecursive *com = new EditLoopRecursive(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::radioSearchOutputAction(int id, int newvalue)
{
    EditSearchOutput *com = new EditSearchOutput(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::radioSearchFileOutputTypeAction(int id, int newvalue)
{
    EditSearchFileOutputType *com = new EditSearchFileOutputType(id, newvalue, cache);

    undostack->push(com);
    emit editUpdate(id);
}





void EditOperator::tableSwapExecAction(int id, int beforeid, int afterid)
{
    SwapExecTable *com = new SwapExecTable(id, beforeid, afterid, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::tableSwapPluginAction(int id, int beforeid, int afterid)
{
    SwapPluginTable *com = new SwapPluginTable(id, beforeid, afterid, cache);

    undostack->push(com);
    emit editUpdate(id);
}





void EditOperator::tableEditVariantAction(int id, int tableid, QStringList variants, int operation)
{
    EditLocalVarTable *com = new EditLocalVarTable(id, tableid, variants, operation, cache);
    undostack->push(com);
//    emit editUpdate(id);? no need to update
}

void EditOperator::tableEditExecAction(int id, int tableid, QString newstr, int operation)
{
    EditExecTable *com = new EditExecTable(id, tableid, newstr, operation, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::tableEditPluginAction(int id, int tableid, QString newstr, int operation)
{
    EditPluginTable *com = new EditPluginTable(id, tableid, newstr, operation, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::tableDragDropExecAction(int id, QList<int> beforeid, int afterid)
{
    DragDropExecTable *com = new DragDropExecTable(id, beforeid, afterid, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::tableDragDropPluginAction(int id, QList<int> beforeid, int afterid)
{
    DragDropPluginTable *com = new DragDropPluginTable(id, beforeid, afterid, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::treeEditGVariantAction(int id, QStringList variants, int operation)
{
    EditGlobalVarTree *com = new EditGlobalVarTree(id, variants, operation);
    undostack->push(com);
}

void EditOperator::treeSwapGVariantAction(int beforeid, int afterid)
{
    SwapGlobalVarTree *com = new SwapGlobalVarTree(beforeid, afterid);
    undostack->push(com);
}

void EditOperator::treeEditLVariantAction(int id, QStringList variants, int operation, int localindex)
{
    EditLocalVariantTree *com = new EditLocalVariantTree(localindex, id, variants, operation, cache);
    undostack->push(com);
}

void EditOperator::treeSwapLVariantAction(int beforeid, int afterid, int localindex)
{
    SwapLocalVariantTree *com = new SwapLocalVariantTree(localindex, beforeid, afterid, cache);
    undostack->push(com);
}






void EditOperator::textFileOutputAction(int id, QString newstr)
{
    StringFileOutput *com = new StringFileOutput(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::textProjectAuthorAction(int id, QString newstr)
{
    StringProjectAuthor *com = new StringProjectAuthor(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::textProjectDescriptAction(int id, QString newstr)
{
    StringProjectDescription *com = new StringProjectDescription(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::textProjectNameAction(int id, QString newstr)
{
    StringProjectName *com = new StringProjectName(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::textProjectVerAction(int id, QString newstr)
{
    StringProjectVersion *com = new StringProjectVersion(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::textSearchSeparateAction(int id, QString newstr)
{
    StringSearchSep *com = new StringSearchSep(id, newstr, cache);

    undostack->push(com);
    emit editUpdate(id);
}





void EditOperator::checkAllowInputAction(int id, bool newcheck)
{
    CheckEditAllowInput *com = new CheckEditAllowInput(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkLoopInfAction(int id, bool newcheck)
{
    CheckEditLoopInf *com = new CheckEditLoopInf(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkSearchInputAction(int id, bool newcheck)
{
    CheckEditSearchInput *com = new CheckEditSearchInput(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkTimeoutAction(int id, bool newcheck)
{
    CheckEditTimeout *com = new CheckEditTimeout(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkRunDetachAction(int id, bool newcheck)
{
    CheckEditRunDetach *com = new CheckEditRunDetach(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkOnlyNormalAction(int id, bool newcheck)
{
    CheckOnlySchedulerNormal *com = new CheckOnlySchedulerNormal(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkOnlySearchAction(int id, bool newcheck)
{
    CheckOnlySchedulerSearch *com = new CheckOnlySchedulerSearch(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkOnlyPluginAction(int id, bool newcheck)
{
    CheckOnlySchedulerPlugin *com = new CheckOnlySchedulerPlugin(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}

void EditOperator::checkOnlyOtherAction(int id, bool newcheck)
{
    CheckOnlySchedulerOther *com = new CheckOnlySchedulerOther(id, newcheck, cache);

    undostack->push(com);
    emit editUpdate(id);
}





void EditOperator::cutAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setListData(cache->at(id));
    clipboard->setMimeData(lmime);

    DeleteCommand *com = new DeleteCommand(id, cache->at(id), cache);
    undostack->push(com);

}

void EditOperator::copyAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setListData(cache->at(id));
    clipboard->setMimeData(lmime);
}

void EditOperator::pasteAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    const ListMimeData *lmime = qobject_cast<const ListMimeData *>(clipboard->mimeData());
    QList<QStringList> tlist(ListMimeData::getListData(lmime));

    insertAction(id, &tlist);
}

void EditOperator::swapAction(int before, int after)
{
    //index regulation
    int ccount = cache->count() - 1;
    if(before == after
            || before < XML_MIN
            || after < XML_MIN
            || before > ccount
            || after > ccount)
        return;

    SwapCommand *com = new SwapCommand(before, after, cache);
    undostack->push(com);
}

void EditOperator::dragDropAction(QList<int> before, int after)
{
    DragDropCommand *com = new DragDropCommand(before, after, cache);
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
    QList<QList<QStringList>> *tempcache = new QList<QList<QStringList>>;
    ProcessXmlListGenerator *xgen = new ProcessXmlListGenerator();
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(autosavefile);

    //optimize xml structure
    int count = updater->count();
    int i;
    QList<QStringList> list;
    for(i = 0; i < count; i++){
        list.clear();
        updater->readItem(i, &list);
        //avoid info, local list
        if(i >= XML_MIN) xgen->createSeparateList(&list);
        tempcache->append(list);
    }

    //full update file
    ProcessXmlBuilder *mexport = new ProcessXmlBuilder();
    mexport->setLoadBlankPath(filepath);

    for(i = 0; i < count; i++){
        mexport->addItem(&(tempcache->at(i)));
    }

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

    //full update file
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadBlankPath(autosavefile);

    int count = cache->count();
    for(int i = 0; i < count; i++){
        updater->addItem(cache->at(i));
    }

    if(loadfile == "") return;

    //append unload index.
    ProcessXmlBuilder *updater2 = new ProcessXmlBuilder();
    updater->setLoadPath(loadfile);

    count = updater2->count();
    QList<QStringList> list;
    for(int i = loadedxmlid; i < count; i++){
        list.clear();
        updater2->readItem(i, &list);
        updater->addItem(&list);
    }

    delete updater2;
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

    updater->readAllItem(cache);
    loadedxmlid = updater->count() - 1;

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

    loadedxmlid = 0;
    autosavefile = "";
    loadfile = "";

    //delete all "new cache"
    qDeleteAll(*cache);
    cache->clear();

    //reset undo action
    undostack->clear();

}

QString EditOperator::getLoadfile() const
{
    return loadfile;
}

int EditOperator::getCacheSize() const
{
    return cache->count();
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
