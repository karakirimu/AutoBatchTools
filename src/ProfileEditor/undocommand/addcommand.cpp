#include "addcommand.h"

AddCommand::AddCommand(const int &targetindex
                       , QList<QStringList> *added
                       , QList<QList<QStringList> *> *cache
                       , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_added = added;
    m_cache = cache;
    m_addcopy.append(*m_added);
}

void AddCommand::undo()
{
    if(m_cache->isEmpty()) return;
    delete m_added;
    m_cache->removeLast();

    setText(QString("Add item at %1 ").arg(m_targetindex) \
            + QString("^(%1)").arg(m_targetindex));
}

void AddCommand::redo()
{
    m_added = new QList<QStringList>(m_addcopy);
    m_cache->append(m_added);

    setText(QString("Add item at %1 ").arg(m_targetindex) \
            + QString("^(%1)").arg(m_targetindex));
}
