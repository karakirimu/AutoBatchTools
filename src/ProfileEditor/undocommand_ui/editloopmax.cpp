#include "editloopmax.h"

EditLoopMax::EditLoopMax(const int &targetindex
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

    m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(8).at(3)).toInt();
}

void EditLoopMax::undo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(8);
    alist.replace(3, QString::number(m_oldvalue));
    m_cache->at(m_targetindex)->replace(8, alist);

    setText(QObject::tr("Loop Max value to %1").arg(m_oldvalue));
}

void EditLoopMax::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(8);
    alist.replace(3, QString::number(m_newvalue));
    m_cache->at(m_targetindex)->replace(8, alist);

    setText(QObject::tr("Loop Max value to %1").arg(m_newvalue));
}

int EditLoopMax::id() const
{
    return ProcessXmlListGenerator::INFO_RLOOP;
}

bool EditLoopMax::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopMax *com = static_cast<const EditLoopMax *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
