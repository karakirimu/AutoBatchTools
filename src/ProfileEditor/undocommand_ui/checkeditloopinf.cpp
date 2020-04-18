#include "checkeditloopinf.h"

CheckEditLoopInf::CheckEditLoopInf(const int &targetindex
                                   , const bool &newcheck
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(I_RECURSIVE_LOOP, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditLoopInf::undo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Loop infinity %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void CheckEditLoopInf::redo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Loop infinity %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}
