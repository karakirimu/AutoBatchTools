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

    m_oldcheck = pxlg.fetch(ALL_TYPE, TYPE_EXEC, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));
}

void CheckOnlySchedulerNormal::undo()
{
    pxlg.replaceTypeElement(TYPE_EXEC, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

void CheckOnlySchedulerNormal::redo()
{
    pxlg.replaceTypeElement(TYPE_EXEC, ATTR_ONLY_SCHEDULER, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

int CheckOnlySchedulerNormal::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_ONLY_SCHEDULER);
}
