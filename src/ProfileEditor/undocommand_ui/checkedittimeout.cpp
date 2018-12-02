#include "checkedittimeout.h"

CheckEditTimeout::CheckEditTimeout(const int &targetindex
                                         , const bool &newcheck
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(E_TIMEOUT, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditTimeout::undo()
{
    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Timeout %1").arg(m_oldcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

void CheckEditTimeout::redo()
{
    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Timeout %1").arg(m_newcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

int CheckEditTimeout::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_TIMEOUT);
}
