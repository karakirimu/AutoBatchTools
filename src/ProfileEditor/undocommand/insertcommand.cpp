#include "insertcommand.h"

InsertCommand::InsertCommand(const int &targetindex
                             , const QList<QStringList> &inserted
                             , QList<QList<QStringList> > *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_inserted = inserted;
    m_cache = cache;
}

void InsertCommand::undo()
{
    if(m_cache->isEmpty()) return;
    m_cache->removeAt(m_targetindex);

    setText(QString("Insert item at %1").arg(m_targetindex));
}

void InsertCommand::redo()
{
    m_cache->insert(m_targetindex, m_inserted);

    setText(QString("Insert item at %1").arg(m_targetindex));
}
