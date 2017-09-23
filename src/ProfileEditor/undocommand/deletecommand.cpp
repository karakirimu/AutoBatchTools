#include "deletecommand.h"

DeleteCommand::DeleteCommand(const int &targetindex
                             , const QList<QStringList> &deleted
                             , QList<QList<QStringList> > *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_deleted = deleted;
    m_cache = cache;
}

void DeleteCommand::undo()
{
    m_cache->insert(m_targetindex, m_deleted);

    setText(QString("Remove item at %1").arg(m_targetindex));
}

void DeleteCommand::redo()
{
    if(m_cache->isEmpty()) return;
    m_cache->removeAt(m_targetindex);

    setText(QString("Remove item at %1").arg(m_targetindex));
}
