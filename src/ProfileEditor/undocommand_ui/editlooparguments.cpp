#include "editlooparguments.h"

EditLoopArguments::EditLoopArguments(const int &targetindex
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

    m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(9).at(1)).toInt();
}

void EditLoopArguments::undo()
{
    QStringList alist;
    alist = m_cache->at(m_targetindex)->at(9);
    alist.replace(1, QString::number(m_oldvalue));
    m_cache->at(m_targetindex)->replace(9, alist);

    setText(QObject::tr("Arguments count to %1").arg(m_oldvalue));
}

void EditLoopArguments::redo()
{
    QStringList alist;
    alist = m_cache->at(m_targetindex)->at(9);
    alist.replace(1, QString::number(m_newvalue));
    m_cache->at(m_targetindex)->replace(9, alist);

    setText(QObject::tr("Arguments count to %1").arg(m_newvalue));
}

int EditLoopArguments::id() const
{
    return ProcessXmlListGenerator::INFO_RLARG;
}

bool EditLoopArguments::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopArguments *com = static_cast<const EditLoopArguments *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
