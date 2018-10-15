#include "editlocalvartable.h"

EditLocalVarTable::EditLocalVarTable(const int &targetindex
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

void EditLocalVarTable::undo()
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

void EditLocalVarTable::redo()
{
    m_cache->replace(m_targetindex, m_changed);

    //locallist
    if(m_targetindex == MAGIC){
        setText(QString("Change table text in Local Variant"));
    }else{
        setText(QString("Change table text at %1").arg(m_targetindex));
    }
}

int EditLocalVarTable::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(L_VARIANT);
}

bool EditLocalVarTable::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLocalVarTable *com = static_cast<const EditLocalVarTable *>(other);
    m_changed = com->m_changed;
    return true;
}
