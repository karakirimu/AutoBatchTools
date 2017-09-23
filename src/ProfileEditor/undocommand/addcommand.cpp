#include "addcommand.h"

AddCommand::AddCommand(const int &targetindex
                       , const QList<QStringList> &added
                       , QList<QList<QStringList>> *cache
                       , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_added = added;
    m_cache = cache;
}

void AddCommand::undo()
{
    if(m_cache->isEmpty()) return;
    m_cache->removeLast();

    setText(QString("Add newitem at %1").arg(m_targetindex));
}

void AddCommand::redo()
{
    m_cache->append(m_added);

    setText(QString("Add newitem at %1").arg(m_targetindex));
}
