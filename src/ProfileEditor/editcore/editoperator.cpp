#include "editoperator.h"
#include "xmllistgenerator.h"

EditOperator::EditOperator(QObject *parent)
    : QObject(parent)
{
    undostack = new QUndoStack();
    cache = new QList<QList<QStringList>*>();

    QSettings settings( "./psettings.ini", QSettings::IniFormat );
    //init timer (ms)
    settings.beginGroup("BASICSETTING");
    timerid = startTimer(settings.value("AUTOSAVEPERIOD", 5).toInt() * 60000);
    settings.endGroup();
}

EditOperator::~EditOperator()
{
    delete undostack;
    delete cache;

    if(autosavefile != ""){
        QFile file(autosavefile);
        if(!file.remove()){
            qDebug() << file.errorString() << file.isOpen();

        }
    }
}

bool EditOperator::isEdited()
{
    return undostack->count() > 0 ? true : false;
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

void EditOperator::addAction()
{
    //create xml inner
    QList<QStringList> xmlstruct;
    ProcessXmlListGenerator *xgen = new ProcessXmlListGenerator();
    xgen->createNewList(&xmlstruct);
    delete xgen;

//    cache->append(xmlstruct);

    AddCommand *com = new AddCommand(cache->count()-1, &xmlstruct, cache);
    undostack->push(com);
    emit edited(isEdited());
//    emit editindexUpdate(cache->count() - 1);
}

void EditOperator::insertAction(int id, QList<QStringList> *xmlstruct)
{
//    cache->insert(id, xmlstruct);

    InsertCommand *com = new InsertCommand(id,xmlstruct,cache);
    undostack->push(com);
    emit edited(isEdited());
//    emit editindexUpdate(id);
}

void EditOperator::deleteAction(int id)
{
    DeleteCommand *com = new DeleteCommand(id,cache->at(id), cache);
//    cache->removeAt(id);

    undostack->push(com);
    emit edited(isEdited());
//    emit editindexUpdate(id);
}

//only temp data accepted
//void EditOperator::editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct)
//{
//     EditCommand *com = new EditCommand(id, innerid, editcode, xmlstruct.at(innerid), cache);
////    cache->replace(id, xmlstruct);

//    undostack->push(com);
//    emit edited(isEdited());
//    //    emit editindexUpdate(id);
//    emit editUpdate(id);
//}

void EditOperator::editTabAction(int id, int newindex)
{
    EditTabCommand *com = new EditTabCommand(id, newindex, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editTextAction(int id, QString mnew, QString obj)
{
    EditTextCommand *com = new EditTextCommand(id, mnew, obj, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editComboBoxAction(int id, QString mnew)
{
    //localvariant combobox only
    EditComboBoxCommand *com = new EditComboBoxCommand(id, mnew, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editFileComboAction(int id, QString newstr, QString newfile, QString obj)
{
    EditFComboBoxCommand *com = new EditFComboBoxCommand(id, newstr, newfile, obj, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editSearchComboAction(int id, QString newstr, int newval)
{
    //search combobox only
    EditScomboBoxCommand *com = new EditScomboBoxCommand(id, newstr, newval, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editValueAction(int id, int newval, QString obj)
{
    EditValueCommand *com = new EditValueCommand(id, newval, obj, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::editCheckAction(int id, bool newcheck, QString obj)
{
    EditCheckCommand *com = new EditCheckCommand(id, newcheck, obj, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

//only temp data accepted
void EditOperator::editVariantAction(int id, QList<QStringList> *xmlstruct)
{
    //record all changes (not merge)
    EditFullCommand *com = new EditFullCommand(id, xmlstruct, cache);
//    cache->replace(id, xmlstruct);

    undostack->push(com);
    emit edited(isEdited());
//    emit editUpdate(id);
    //    emit editindexUpdate(id);
}

void EditOperator::editTableAction(int id, int tableid, QString newstr, int operation, QString objname)
{
    EditTableCommand *com = new EditTableCommand(id, tableid, newstr, operation, objname, cache);

    undostack->push(com);
    emit edited(isEdited());
    emit editUpdate(id);
}

void EditOperator::cutAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setList(cache->at(id));
    clipboard->setMimeData(lmime);

    DeleteCommand *com = new DeleteCommand(id, cache->at(id), cache);
    undostack->push(com);

//    cache->removeAt(id);
    emit edited(isEdited());
//    emit editindexUpdate(id);
}

void EditOperator::copyAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setList(cache->at(id));
    clipboard->setMimeData(lmime);
}

void EditOperator::pasteAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();

    //TODO:
    if(clipboard->ownsClipboard()
            && clipboard->mimeData()->hasFormat(QLatin1String("application/x-qt-profilerlist"))){
        const ListMimeData *lmime = qobject_cast<const ListMimeData *>(clipboard->mimeData());

//        const QList<QStringList> list = lmime->list();

        //copy
//        QList<QStringList> lst = const_cast<QList<QStringList>>(lmime->list());

        //encode
        QByteArray dat = lmime->data(QLatin1String("application/x-qt-profilerlist"));
        QString dats = QString::fromLocal8Bit(dat);
        QStringList ilist = dats.split("\n");

        QList<QStringList> _list;
        QStringList tmp;
        int count = ilist.count();
        for(int i = 0; i < count - 1; i++){
            tmp.clear();
            tmp = ((QString)ilist.at(i)).split("\\\" \\\"");
            tmp.replace(0, ((QString)tmp.at(0)).remove(0,2));
            QString inner = ((QString)tmp.at(tmp.count() - 1));
            tmp.replace(tmp.count() - 1, inner.left(inner.size() - 4));
            _list.append(tmp);
        }

//        QVariant data(dat);
//        QVariantList list = data.toList();
//        QList<QStringList> _list;

//        QMutableListIterator<QVariant> i(list);
//        while(i.hasNext()){
//            _list.append(i.next().toStringList());
//        }

        insertAction(id, &_list);

//        cache->insert(id, lmime->list());
        emit edited(isEdited());
//        emit editindexUpdate(id);
    }
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

    //swap id
//    QList<QStringList> tmp = cache->at(after);
//    cache->replace(after, cache->at(before));
//    cache->replace(before, tmp);

    SwapCommand *com = new SwapCommand(before, after, cache);
    undostack->push(com);

    emit edited(isEdited());
}

void EditOperator::swapTableAction(int id, int beforeid, int afterid, QString objname)
{
    SwapTableCommand *com = new SwapTableCommand(id, beforeid, afterid, objname, cache);

    undostack->push(com);
    emit edited(isEdited());
}

void EditOperator::newAction()
{
    reset();
    autosavefile = "./.~$" + QString::number(QDateTime::currentSecsSinceEpoch()).append(".autosave");

    //create xmlfile
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(autosavefile);
    delete updater;

    //temp loadfile change
    loadfile = autosavefile;
    loadcache(XML_MIN);
    loadfile = "";

    emit loadfileChanged(autosavefile);
    emit edited(false);

    //reset undo action
    undostack->clear();
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

    emit loadfileChanged(loadfile);
    emit edited(false);

    //reset undo action
    undostack->clear();

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
    emit edited(false);
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

    for(int i = 0; i < count; i++){
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
        QFile::remove(autosavefile);
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

void EditOperator::loadcache(int amount)
{
    if(loadfile == "") return;

    //append new index along with "amount" number.
    ProcessXmlBuilder *updater = new ProcessXmlBuilder();
    updater->setLoadPath(loadfile);

    int count = updater->count();

    //load all data (TODO:not efficient)
    if(amount == 0) amount = count;
    int i;
    QList<QStringList> *list;
    for(i = loadedxmlid; i < count; i++){
        list = new QList<QStringList>();
        if(i < amount){
            updater->readItem(i, list);
            cache->append(list);
        }else{
            break;
        }
    }

    loadedxmlid = i - 1;

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
}

int EditOperator::getCacheSize() const
{
    return cache->count();
}

int EditOperator::getCurrentCommandType()
{
    qDebug() << "stack : " << undostack->index();
    return undostack->command(undostack->index())->id();
}

QUndoStack *EditOperator::getUndostack() const
{
    return undostack;
}

void EditOperator::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerid){
        //autosave commands
        qDebug() << "EditOperator::save()";
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
