#include "checkonlyschedulersearch.h"

CheckOnlySchedulerSearch::CheckOnlySchedulerSearch(const int &targetindex
                                                   , const bool &newcheck
                                                   , QList<QList<QStringList> *> *cache
                                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(ALL_TYPE, TYPE_SEARCH, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));
}

void CheckOnlySchedulerSearch::undo()
{
    pxlg.replaceTypeElement(TYPE_SEARCH, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void CheckOnlySchedulerSearch::redo()
{
    pxlg.replaceTypeElement(TYPE_SEARCH, ATTR_ONLY_SCHEDULER, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int CheckOnlySchedulerSearch::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(S_ONLY_SCHEDULER);
}
