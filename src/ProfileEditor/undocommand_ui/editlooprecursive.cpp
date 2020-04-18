#include "editlooprecursive.h"

EditLoopRecursive::EditLoopRecursive(const int &targetindex
                                     , int newvalue
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    m_oldvalue = static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();

}

void EditLoopRecursive::undo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Loop recursive count to %1").arg(m_newvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void EditLoopRecursive::redo()
{
    pxlg.replaceElementList(I_RECURSIVE_LOOPCOUNT, ATTR_NONE, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Loop recursive count to %1").arg(m_oldvalue) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int EditLoopRecursive::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_RECURSIVE_LOOPCOUNT);
}

bool EditLoopRecursive::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopRecursive *com = static_cast<const EditLoopRecursive *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
