#include "stringsearchsep.h"

StringSearchSep::StringSearchSep(const int &targetindex
                                         , QString newstring
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    m_oldstring = pxlg.fetch(S_SEPARATOR, ATTR_NONE, m_cache->at(m_targetindex));

}

void StringSearchSep::undo()
{
    pxlg.replaceElementList(S_SEPARATOR, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Search Sep '%1'").arg(m_newstring) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void StringSearchSep::redo()
{
    pxlg.replaceElementList(S_SEPARATOR, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Search Sep '%1'").arg(m_oldstring) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int StringSearchSep::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(S_SEPARATOR);
}

bool StringSearchSep::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringSearchSep *com = static_cast<const StringSearchSep*>(other);
    m_newstring = com->m_newstring;
    return true;
}
