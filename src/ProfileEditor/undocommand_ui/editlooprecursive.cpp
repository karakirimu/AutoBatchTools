#include "editlooprecursive.h"

EditLoopRecursive::EditLoopRecursive(const int &targetindex
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

    m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(10).at(1)).toInt();
}

void EditLoopRecursive::undo()
{
    QStringList alist;
    alist = m_cache->at(m_targetindex)->at(10);
    alist.replace(1, QString::number(m_oldvalue));
    m_cache->at(m_targetindex)->replace(10, alist);

    setText(QObject::tr("Loop recursive value to %1").arg(m_oldvalue));
}

void EditLoopRecursive::redo()
{
    QStringList alist;
    alist = m_cache->at(m_targetindex)->at(10);
    alist.replace(1, QString::number(m_newvalue));
    m_cache->at(m_targetindex)->replace(10, alist);

    setText(QObject::tr("Loop recursive value to %1").arg(m_newvalue));
}

int EditLoopRecursive::id() const
{
    return ProcessXmlListGenerator::INFO_RELOOP;
}

bool EditLoopRecursive::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopRecursive *com = static_cast<const EditLoopRecursive *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
