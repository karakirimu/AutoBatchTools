#include "checkonlyschedulernormal.h"

CheckOnlySchedulerNormal::CheckOnlySchedulerNormal(const int &targetindex
                                                   , const bool &newcheck
                                                   , QList<QList<QStringList> *> *cache
                                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    if(m_targetindex > 1){
        //init generator
        ProcessXmlListGenerator x;
        x.getListStructure(m_cache->at(m_targetindex), &xmlpos);
    }

    m_oldcheck = m_cache->at(m_targetindex)->at(getxmlpos()).at(SUBPOS);
}

void CheckOnlySchedulerNormal::undo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
    alist.replace(SUBPOS, m_oldcheck);

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Scheduler only %1").arg(m_oldcheck));
}

void CheckOnlySchedulerNormal::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
    alist.replace(SUBPOS, m_newcheck);

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Scheduler only %1").arg(m_newcheck));
}

int CheckOnlySchedulerNormal::getxmlpos()
{
    return xmlpos.value(ProcessXmlListGenerator::NORMAL);
}
