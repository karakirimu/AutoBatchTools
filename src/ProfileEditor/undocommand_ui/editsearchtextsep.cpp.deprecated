#include "editsearchtextsep.h"

EditSearchTextSep::EditSearchTextSep(const int &targetindex
                                     , QString newstring
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;

    m_oldstring = static_cast<QString>(pxlg.fetch(S_SEPARATOR, ATTR_NONE, m_cache->at(m_targetindex)));

}

void EditSearchTextSep::undo()
{
    pxlg.replaceElementList(S_SEPARATOR, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Change text at Search Separator") \
            + QString("^(%1)").arg(m_targetindex));
}

void EditSearchTextSep::redo()
{
    pxlg.replaceElementList(S_SEPARATOR, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Change text at Search Separator") \
            + QString("^(%1)").arg(m_targetindex));
}

int EditSearchTextSep::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(S_SEPARATOR);
}

bool EditSearchTextSep::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchTextSep *com = static_cast<const EditSearchTextSep*>(other);
    m_newstring = com->m_newstring;
    return true;
}
