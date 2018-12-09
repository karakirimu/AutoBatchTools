#include "addcommand.h"

AddCommand::AddCommand(const int &targetindex
                       , QList<QStringList> *added
                       , QList<QList<QStringList> *> *cache
                       , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
//    m_added = added;
    m_cache = cache;
    m_addcopy.append(*added);
}

void AddCommand::undo()
{
    if(m_cache->isEmpty()) return;

//    delete m_added;
    m_cache->removeAt(m_targetindex);

    setText(QObject::tr("Add item at %1").arg(m_targetindex) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_DELETE));
}

void AddCommand::redo()
{
//    m_added = ;
    m_cache->append(new QList<QStringList>(m_addcopy));

    setText(QObject::tr("Add item at %1").arg(m_targetindex) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_ADD));
}
