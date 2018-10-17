#include "stringprojectdescription.h"

StringProjectDescription::StringProjectDescription(const int &targetindex
                                                   , QString newstring
                                                   , QList<QList<QStringList> *> *cache
                                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;
    m_oldstring = pxlg.fetch(I_DESCRIPTION, ATTR_NONE, m_cache->at(m_targetindex));
}

void StringProjectDescription::undo()
{
    pxlg.replaceElementList(I_DESCRIPTION, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Description Text ") \
            + QString("^(%1)").arg(m_targetindex));
}

void StringProjectDescription::redo()
{
    pxlg.replaceElementList(I_DESCRIPTION, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Description Text ") \
            + QString("^(%1)").arg(m_targetindex));
}

int StringProjectDescription::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_DESCRIPTION);
}

bool StringProjectDescription::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectDescription *com = static_cast<const StringProjectDescription*>(other);
    m_newstring = com->m_newstring;
    return true;
}
