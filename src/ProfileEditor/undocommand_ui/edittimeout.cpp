#include "edittimeout.h"

EditTimeout::EditTimeout(const int &targetindex
                         , int newvalue
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    if(m_targetindex > 1){
        //init generator
        ProcessXmlListGenerator x;
        x.getListStructure(m_cache->at(m_targetindex), &posinfo);
    }

    //get old value
    m_oldvalue = ((QString)m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1)
                .at(3)).toInt();
}

void EditTimeout::undo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
    alist.replace(3, QString::number(m_oldvalue));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);

    setText(QObject::tr("Timeout value to %1").arg(m_oldvalue));
}

void EditTimeout::redo()
{
    QStringList alist;
    alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
    alist.replace(3, QString::number(m_newvalue));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);

    setText(QObject::tr("Timeout value to %1").arg(m_newvalue));
}

int EditTimeout::id() const
{
    return ProcessXmlListGenerator::TIMEOUT_DURITION;
}

bool EditTimeout::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTimeout *com = static_cast<const EditTimeout *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
