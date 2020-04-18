#include "checkonlyschedulerplugin.h"

CheckOnlySchedulerPlugin::CheckOnlySchedulerPlugin(const int &targetindex
                                                   , const bool &newcheck
                                                   , QList<QList<QStringList> *> *cache
                                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(ALL_TYPE, TYPE_SCRIPT, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));
}

void CheckOnlySchedulerPlugin::undo()
{
    pxlg.replaceTypeElement(TYPE_SCRIPT, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void CheckOnlySchedulerPlugin::redo()
{
    pxlg.replaceTypeElement(TYPE_SCRIPT, ATTR_ONLY_SCHEDULER, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Scheduler only %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int CheckOnlySchedulerPlugin::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(PL_ONLY_SCHEDULER);
}
