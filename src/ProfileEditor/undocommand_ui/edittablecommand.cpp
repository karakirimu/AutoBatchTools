#include "edittablecommand.h"

EditTableCommand::EditTableCommand(const int &targetindex
                                   , const int &tableindex
                                   , QString newstr
                                   , const int operation
                                   , const QString objname
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_tableindex = tableindex;
    m_newstr = newstr;
    m_oldstr = "";
    m_operation = operation;
    m_objname = objname;
    m_cache = cache;

    //init generator
    ProcessXmlListGenerator x;
    QHash<int, int> posinfo;
    x.getListStructure(m_cache->at(m_targetindex), &posinfo);

    switch (id()) {
    case ProcessXmlListGenerator::NCMDCOUNT:
        //define SKIP
        SKIP = posinfo.value(ProcessXmlListGenerator::NORMAL) + 3;

        if(m_operation == ProcessXmlListGenerator::TABLE_EDIT
                || m_operation == ProcessXmlListGenerator::TABLE_DELETE){
        m_oldstr = m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(1);
        }

        break;
    case ProcessXmlListGenerator::ECMDCOUNT:
        //define SKIP
        SKIP = posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 4;

        if(m_operation == ProcessXmlListGenerator::TABLE_EDIT
                || m_operation == ProcessXmlListGenerator::TABLE_DELETE){
        m_oldstr = m_cache->at(m_targetindex)->at(m_tableindex + SKIP).at(1);
        }
        break;
    default:
        break;
    }

}

void EditTableCommand::undo()
{
    int rcount = -1;
    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Add table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        alist = m_cache->at(m_targetindex)->at(m_tableindex + SKIP);
        alist.replace(1, m_oldstr);
        m_cache->at(m_targetindex)->replace(m_tableindex + SKIP, alist);

        setText(QObject::tr("Set table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:
        rcount = ((QString)m_cache->at(m_targetindex)->at(SKIP - 1).at(1)).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Insert table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        rcount = ((QString)m_cache->at(m_targetindex)->at(SKIP - 1).at(1)).toInt();
        alist = ProcessXmlListGenerator::createCmdElement(m_oldstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Delete table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    default:
        break;
    }
}

void EditTableCommand::redo()
{
    QStringList alist;
    int rcount = -1;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        alist = ProcessXmlListGenerator::createCmdElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Add table at %1").arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        alist = m_cache->at(m_targetindex)->at(m_tableindex + SKIP);
        alist.replace(1, m_newstr);
        m_cache->at(m_targetindex)->replace(m_tableindex + SKIP, alist);

        setText(QObject::tr("Set table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:       
        rcount = ((QString)m_cache->at(m_targetindex)->at(SKIP - 1).at(1)).toInt();
        alist = ProcessXmlListGenerator::createCmdElement(m_newstr, m_tableindex);
        m_cache->at(m_targetindex)->insert(m_tableindex + SKIP, alist);

        updateCounter(true);

        if(m_tableindex < rcount) updateIndex(rcount);

        setText(QObject::tr("Insert table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        rcount = ((QString)m_cache->at(m_targetindex)->at(SKIP - 1).at(1)).toInt();
        m_cache->at(m_targetindex)->removeAt(m_tableindex + SKIP);

        updateCounter(false);

        if(m_tableindex < (rcount-1)) updateIndex(rcount-1);

        setText(QObject::tr("Delete table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    default:
        break;
    }
}

//DEPENDS_XML DEPENDS_UI
int EditTableCommand::id() const
{
    //TODO: it may cause doing "margewith" other indexs id `
    // (added addcommand between two tableadd but editcommand unchecked)
    if(m_objname == "cmdTableWidget"){
        return ProcessXmlListGenerator::NCMDCOUNT;
    }else if(m_objname == "extrafuncTableWidget"){
        return ProcessXmlListGenerator::ECMDCOUNT;
    }else{
        return -1;
    }
}

bool EditTableCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTableCommand *com = static_cast<const EditTableCommand *>(other);
    if(operation() == ProcessXmlListGenerator::TABLE_EDIT){
        m_newstr = com->m_newstr;
    }
    return true;
}

void EditTableCommand::updateIndex(int count)
{
    QStringList alist;
    for(int i = m_tableindex; i < count; i++){
        alist = m_cache->at(m_targetindex)->at(i + SKIP);
        alist.replace(3, QString::number(i));
        m_cache->at(m_targetindex)->replace(i + SKIP, alist);

    }
}

void EditTableCommand::updateCounter(bool ascend)
{
    QStringList alist;
    //index size update
    alist = m_cache->at(m_targetindex)->at(SKIP - 1);
    int rcount = ((QString)alist.at(1)).toInt();
    if(ascend){
        alist.replace(1, QString::number(rcount + 1));
    }else{
        alist.replace(1, QString::number(rcount - 1));
    }
    m_cache->at(m_targetindex)->replace(SKIP - 1, alist);
}

int EditTableCommand::operation() const
{
    return m_operation;
}
