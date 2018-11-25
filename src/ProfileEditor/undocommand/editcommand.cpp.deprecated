#include "editcommand.h"

EditCommand::EditCommand(const int &targetindex
                                     , const int &xmllistnum
                                     , const int &editid
                                     , const QStringList &after
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_xmllistnum = xmllistnum;
    m_editid = editid;
    m_before = cache->at(targetindex)->at(xmllistnum);
    m_after = after;
    m_cache = cache;
}

void EditCommand::undo()
{
    if(m_cache->isEmpty()) return;

    //replace inserted num
    QList<QStringList> *inner = m_cache->at(m_targetindex);
    inner->replace(m_xmllistnum, m_before);

    //replace parent Qhash
    m_cache->replace(m_targetindex, inner);

    //explain text TODO:no attributes
    setText(QString("Change text in %1").arg(m_targetindex));
}

void EditCommand::redo()
{
    //replace inserted num
    QList<QStringList> *inner = m_cache->at(m_targetindex);
    inner->replace(m_xmllistnum, m_after);

    //replace parent Qhash
    m_cache->replace(m_targetindex, inner);

    //explain text
    //explain text TODO:no attributes
    setText(QString("Change text in %1").arg(m_targetindex));
}

int EditCommand::id() const
{
    return m_editid;
}

bool EditCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    m_after = static_cast<const EditCommand*>(other)->after();
    return true;
}

QStringList EditCommand::after() const
{
    return m_after;
}
