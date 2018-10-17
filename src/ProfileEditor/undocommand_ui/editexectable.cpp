#include "editexectable.h"

EditExecTable::EditExecTable(const int &targetindex
                                       ,const int &tableindex
                                       ,QString newstr
                                       ,const int operation
                                       ,QList<QList<QStringList> *> *cache
                                       ,QUndoCommand *parent)
    :QUndoCommand (parent)
{
    m_targetindex = targetindex;
    m_tableindex = tableindex;
    m_newstr = newstr;
    m_oldstr = "";
    m_operation = operation;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(ALL_CMDVALUE,ProcessXmlListGenerator::NORMAL, m_cache->at(m_targetindex));

    if(m_operation == ProcessXmlListGenerator::TABLE_EDIT
            || m_operation == ProcessXmlListGenerator::TABLE_DELETE){
        m_oldstr = m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(1);
    }

}

void EditExecTable::undo()
{
    int rcount = -1;
    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Add exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_EDIT:

        pxlg.replaceElementList(m_tableindex, m_targetindex, m_oldstr, SKIP, m_cache);

        setText(QObject::tr("Edit exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Insert exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createCmdElement(m_oldstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Delete exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    default:
        break;
    }
}

void EditExecTable::redo()
{
    QStringList alist;
    int rcount = -1;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        alist = ProcessXmlListGenerator::createCmdElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Add exec arg at %1 ").arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        pxlg.replaceElementList(m_tableindex, m_targetindex, m_newstr, SKIP, m_cache);

        setText(QObject::tr("Edit exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        alist = ProcessXmlListGenerator::createCmdElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Insert exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Delete exec arg \'%1\' at %2 ").arg(m_newstr).arg(QString::number(m_tableindex)) \
                + QString("^(%1)").arg(m_targetindex));
        break;
    default:
        break;
    }
}

int EditExecTable::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_CMDARGCOUNT);
}

bool EditExecTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditExecTable *com = static_cast<const EditExecTable *>(other);
    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
        m_newstr = com->m_newstr;
    }
    return true;
}

int EditExecTable::operation() const
{
    return m_operation;
}

void EditExecTable::updateIndex(int count)
{
    QStringList alist;
    for(int i = m_tableindex; i < count; i++){
        alist = m_cache->at(m_targetindex)->at(i + SKIP);
        alist.replace(3, QString::number(i));
        m_cache->at(m_targetindex)->replace(i + SKIP, alist);

    }
}

void EditExecTable::updateCounter(bool ascend)
{
    QStringList alist;
    //index size update
    alist = m_cache->at(m_targetindex)->at(SKIP - 1);
    int rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    if(ascend){
        alist.replace(1, QString::number(rcount + 1));
    }else{
        alist.replace(1, QString::number(rcount - 1));
    }
    m_cache->at(m_targetindex)->replace(SKIP - 1, alist);
}
