#include "swapcommand.h"

SwapCommand::SwapCommand(const int &beforeindex
                         , const int &afterindex
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_beforeindex = beforeindex;
    m_afterindex = afterindex;
    m_cache = cache;
}

void SwapCommand::undo()
{
    if(m_cache->isEmpty()) return;
    QList<QStringList> *tmp = m_cache->at(m_afterindex);
    m_cache->replace(m_afterindex, m_cache->at(m_beforeindex));
    m_cache->replace(m_beforeindex, tmp);

    setText(QString("Swap item at %1 and %2").arg(m_beforeindex).arg(m_afterindex));
}

void SwapCommand::redo()
{
    if(m_cache->isEmpty()) return;
    QList<QStringList> *tmp = m_cache->at(m_beforeindex);

    m_cache->replace(m_beforeindex, m_cache->at(m_afterindex));
    m_cache->replace(m_afterindex, tmp);

    setText(QString("Swap item at %1 and %2").arg(m_beforeindex).arg(m_afterindex));
}
