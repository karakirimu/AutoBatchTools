#include "insertcommand.h"

InsertCommand::InsertCommand(const int &targetindex
                             , QList<QStringList> *inserted
                             , QList<QList<QStringList> *> *cache
                             , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_inserted = inserted;
    m_inscopy.append(*inserted);
    m_cache = cache;
}

void InsertCommand::undo()
{
    if(m_cache->isEmpty()) return;
    delete m_inserted;
    m_cache->removeAt(m_targetindex);

    setText(QString("Insert item at %1 ").arg(m_targetindex) \
            + QString("^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_DELETE));
}

void InsertCommand::redo()
{
    m_inserted = new QList<QStringList>(m_inscopy);
    m_cache->insert(m_targetindex, m_inserted);

    setText(QString("Insert item at %1 ").arg(m_targetindex) \
            + QString("^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_INSERT));
}
