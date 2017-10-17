#include "editfullcommand.h"

EditFullCommand::EditFullCommand(const int &targetindex
                                 , QList<QStringList> *changed
                                 , QList<QList<QStringList> *> *cache
                                 , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_changed = changed;
    m_before = cache->at(targetindex);
    m_cache = cache;
}

void EditFullCommand::undo()
{
    if(m_cache->isEmpty()) return;
    m_cache->replace(m_targetindex, m_before);

    //locallist
    if(m_targetindex == MAGIC){
        setText(QString("Change table text in Local Variant"));
    }else{
        setText(QString("Change table text at %1").arg(m_targetindex));
    }
}

void EditFullCommand::redo()
{
    m_cache->replace(m_targetindex, m_changed);

    //locallist
    if(m_targetindex == MAGIC){
        setText(QString("Change table text in Local Variant"));
    }else{
        setText(QString("Change table text at %1").arg(m_targetindex));
    }
}
