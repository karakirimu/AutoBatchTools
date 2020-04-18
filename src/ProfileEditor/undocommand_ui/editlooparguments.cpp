#include "editlooparguments.h"

EditLoopArguments::EditLoopArguments(const int &targetindex
                                     , int newvalue
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    m_oldvalue = static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
}

void EditLoopArguments::undo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Arguments count to %1").arg(m_newvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void EditLoopArguments::redo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Arguments count to %1").arg(m_oldvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int EditLoopArguments::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_RECURSIVE_LOOPARGCOUNT);
}

bool EditLoopArguments::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopArguments *com = static_cast<const EditLoopArguments *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
