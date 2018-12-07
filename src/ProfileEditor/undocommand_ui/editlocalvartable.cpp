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
    qDebug() << "VariantTable: Undo: tableindex : " << m_tableindex;

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
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_tableindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEDEL));
        break;

    case ProcessXmlListGenerator::TABLE_DELETE:
        alist = ProcessXmlListGenerator::createVariantElement(m_oldvar);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

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
    qDebug() << "VariantTable: Redo: tableindex : " << m_tableindex;


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

        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_tableindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_tableindex).arg(UNDOREDO_L_TABLEINS));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        //add
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Delete local at %1").arg(m_tableindex) \
                + QString(" ^(%1,%2)").arg(m_tableindex).arg(UNDOREDO_L_TABLEDEL));
        break;
    default:
        break;
    }

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
    Q_UNUSED(other);
//    if (other->id() != id()) return false;
//    const EditLocalVarTable *com = static_cast<const EditLocalVarTable *>(other);
//    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
//        m_newvar = com->m_newvar;
//    }else{
//        return false;
//    }
//    return true;
    return false;
}

//int EditLocalVarTable::operation() const
//{
//    return m_operation;
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
