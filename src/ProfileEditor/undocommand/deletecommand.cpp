#include "deletecommand.h"

DeleteCommand::DeleteCommand(const int &targetindex
                             , QList<QStringList> *deleted
                             , QList<QList<QStringList> *> *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_deleted = deleted;
    m_delcopy.append(*deleted);
    m_cache = cache;

}

void DeleteCommand::undo()
{
    m_deleted = new QList<QStringList>(m_delcopy);
    m_cache->insert(m_targetindex, m_deleted);

    setText(QString("Remove item at %1 ").arg(m_targetindex) \
            + QString("^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_INSERT));
}

void DeleteCommand::redo()
{
    if(m_cache->isEmpty()) return;

    delete m_deleted;
    m_cache->removeAt(m_targetindex);

    setText(QString("Remove item at %1 ").arg(m_targetindex) \
            + QString("^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_DELETE));
}
