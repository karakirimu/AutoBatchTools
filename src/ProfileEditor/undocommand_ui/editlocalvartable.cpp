#include "editlocalvartable.h"

EditLocalVarTable::EditLocalVarTable(const int &targetindex
                                     , const int &tableindex
                                     , QStringList newstrlist
                                     , const int operation
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_tableindex = tableindex;

    m_newvar = newstrlist;
    m_oldvar.clear();

    m_operation = operation;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(L_VAR_COUNT, m_cache->at(m_targetindex));

    if(m_operation == ProcessXmlListGenerator::TABLE_EDIT
            || m_operation == ProcessXmlListGenerator::TABLE_DELETE){
        m_oldvar.append(m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(1));
        m_oldvar.append(m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(3));
    }
}

void EditLocalVarTable::undo()
{
//    if(m_cache->isEmpty()) return;
//    m_cache->replace(m_targetindex, m_before);

//    //locallist
//    if(m_targetindex == MAGIC){
//        setText(QString("Change table text in Local Variant"));
//    }else{
//        setText(QString("Change table text at %1").arg(m_targetindex));
//    }
//    int rcount = -1;
    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Add local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEDEL));

        break;

    case ProcessXmlListGenerator::TABLE_EDIT:
        pxlg.replaceElementList(m_tableindex, m_targetindex, m_oldvar, SKIP, m_cache);

        setText(QObject::tr("Edit local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_L_TABLEEDIT));
        break;

    case ProcessXmlListGenerator::TABLE_INSERT:
//        rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

//        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_tableindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEDEL));
        break;

    case ProcessXmlListGenerator::TABLE_DELETE:
//        rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createVariantElement(m_oldvar);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

//        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Delete local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_L_TABLEINS));
        break;

    default:
        break;
    }
}

void EditLocalVarTable::redo()
{
    QStringList alist;
//    int rcount = -1;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Add local %1").arg(m_tableindex) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEADD));

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        pxlg.replaceElementList(m_tableindex, m_targetindex, m_newvar, SKIP, m_cache);

        setText(QObject::tr("Edit local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_L_TABLEEDIT));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:

//        rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

//        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_tableindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_L_TABLEINS));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        //add
//        rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

//        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Delete local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEDEL));
        break;
    default:
        break;
    }

//    m_cache->replace(m_targetindex, m_changed);

//    //locallist
//    if(m_targetindex == MAGIC){
//        setText(QString("Change table text in Local Variant"));
//    }else{
//        setText(QString("Change table text at %1").arg(m_targetindex));
//    }
}

int EditLocalVarTable::id() const
{
    ProcessXmlListGenerator pxg;

    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        return pxg.getId(L_ADD_TABLE);

    case ProcessXmlListGenerator::TABLE_EDIT:
        return pxg.getId(L_EDIT_TABLE);

    case ProcessXmlListGenerator::TABLE_INSERT:
        return pxg.getId(L_INSERT_TABLE);

    case ProcessXmlListGenerator::TABLE_DELETE:
        return pxg.getId(L_DELETE_TABLE);

    }

    return pxg.getId(L_VAR_COUNT);
}

bool EditLocalVarTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLocalVarTable *com = static_cast<const EditLocalVarTable *>(other);
//    m_newval = com->m_newval;
    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
        m_newvar = com->m_newvar;
    }else{
        return false;
    }
    return true;
}

int EditLocalVarTable::operation() const
{
    return m_operation;
}

//void EditLocalVarTable::updateIndex(int count)
//{
//    QStringList alist;
//    for(int i = m_tableindex; i < count; i++){
//        alist = m_cache->at(m_targetindex)->at(i + SKIP);
//        alist.replace(3, QString::number(i));
//        m_cache->at(m_targetindex)->replace(i + SKIP, alist);

//    }
//}

void EditLocalVarTable::updateCounter(bool ascend)
{
    QStringList alist;
    //index size update
    alist = m_cache->at(m_targetindex)->at(SKIP - 1);
    int rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    if(ascend){
        alist.replace(1, QString::number(rcount + 1));
    }else{
        alist.replace(1, QString::number(rcount - 1));
    }
    m_cache->at(m_targetindex)->replace(SKIP - 1, alist);
}
