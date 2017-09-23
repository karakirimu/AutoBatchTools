#include "editoperator.h"
#include "xmllistgenerator.h"

EditOperator::EditOperator(QObject *parent)
    : QObject(parent)
{
    undostack = new QUndoStack();
    cache = new QList<QList<QStringList>>();

    //init timer (ms)
    timerid = startTimer(600000);
}

EditOperator::~EditOperator()
{
    delete undostack;
    delete cache;
}

bool EditOperator::isEdited()
{
    return undostack->count() > 0 ? true : false;
}

QList<QStringList> EditOperator::read(int id)
{
    return cache->at(id);
}

void EditOperator::addAction(QList<QStringList> xmlstruct)
{
    cache->append(xmlstruct);

    AddCommand *com = new AddCommand(cache->count()-1, xmlstruct, cache);
    undostack->push(com);
    emit edited(isEdited());
    emit editindexUpdate(cache->count() - 1);
}

void EditOperator::insertAction(int id, QList<QStringList> xmlstruct)
{
    cache->insert(id, xmlstruct);

    InsertCommand *com = new InsertCommand(id,xmlstruct,cache);
    undostack->push(com);
    emit edited(isEdited());
    emit editindexUpdate(id);
}

void EditOperator::deleteAction(int id)
{
    DeleteCommand *com = new DeleteCommand(id,cache->at(id), cache);
    cache->removeAt(id);

    undostack->push(com);
    emit edited(isEdited());
    emit editindexUpdate(id);
}

void EditOperator::editAction(int id, int innerid, int editcode, QList<QStringList> xmlstruct)
{
    EditCommand *com = new EditCommand(id, innerid, editcode, xmlstruct.at(innerid), cache);
    cache->replace(id, xmlstruct);

    undostack->push(com);
    emit edited(isEdited());
    emit editindexUpdate(id);
}

void EditOperator::editFullAction(int id, QList<QStringList> xmlstruct)
{
    //record all changes (not merge)
    EditFullCommand *com = new EditFullCommand(id, xmlstruct, cache);
    cache->replace(id, xmlstruct);

    undostack->push(com);
    emit edited(isEdited());
    emit editindexUpdate(id);
}

void EditOperator::cutAction(int id)
{
    QClipboard *clipboard = QApplication::clipboard();
    ListMimeData *lmime = new ListMimeData();

    lmime->setList(cache->at(id));
    clipboard->setMimeData(lmime);

    cache->removeAt(id);
    emit edited(isEdited());
    emit editindexUpdate(id);
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

    if(clipboard->ownsClipboard()
            && clipboard->mimeData()->hasFormat(QLatin1String("application/apro"))){
        const ListMimeData *lmime = qobject_cast<const ListMimeData *>(clipboard->mimeData());
        cache->insert(id, lmime->list());
        emit edited(isEdited());
        emit editindexUpdate(id);
    }
}

void EditOperator::newAction()
{
    reset();
    autosavefile = "/.~$" + QString::number(QDateTime::currentSecsSinceEpoch()).append(".autosave");
    loadcache(XML_MIN);

    emit loadfileChanged(autosavefile);
}

void EditOperator::openAction(QString filepath)
{
    reset();

    //generate copy file
    QFileInfo info(filepath);
    autosavefile = "/.~$" + info.fileName();
    loadfile = filepath;

    //copy file
    QFile::copy(filepath, autosavefile);

    //load action TODO:init loading num
    loadcache(50);

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
    XmlListGenerator *xgen = new XmlListGenerator();
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

    delete updater;
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
    updater->setLoadPath(autosavefile);

    int count = cache->count();
    for(int i = 0; i < count; i++){
        updater->addItem(&(cache->at(i)));
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
    int i;
    QList<QStringList> list;
    for(i = loadedxmlid; i < count; i++){
        list.clear();
        if(i < amount){
            updater->readItem(i, &list);
            cache->append(list);
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
    cache->clear();
}

void EditOperator::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerid){
        //autosave commands
        qDebug() << "EditOperator::save()";
        save();
    }
}
