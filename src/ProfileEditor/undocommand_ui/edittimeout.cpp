#include "edittimeout.h"

EditTimeout::EditTimeout(const int &targetindex
                         , int newvalue
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    m_oldvalue = static_cast<QString>(pxlg.fetch(E_TIMEOUT, ATTR_TIMEOUTMS, m_cache->at(m_targetindex))).toInt();
}

void EditTimeout::undo()
{
    pxlg.replaceElementList(E_TIMEOUT, ATTR_TIMEOUTMS, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Timeout to %1ms").arg(m_newvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void EditTimeout::redo()
{
    pxlg.replaceElementList(E_TIMEOUT, ATTR_TIMEOUTMS, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Timeout to %1ms").arg(m_oldvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int EditTimeout::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_TIMEOUT);
}

bool EditTimeout::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTimeout *com = static_cast<const EditTimeout *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
