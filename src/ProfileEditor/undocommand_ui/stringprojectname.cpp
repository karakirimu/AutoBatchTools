#include "stringprojectname.h"

StringProjectName::StringProjectName(const int &targetindex
                                     , QString newstring
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;
    m_oldstring = pxlg.fetch(I_NAME, ATTR_NONE, m_cache->at(m_targetindex));
}

void StringProjectName::undo()
{
    pxlg.replaceElementList(I_NAME, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Project name changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

void StringProjectName::redo()
{
    pxlg.replaceElementList(I_NAME, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Project name changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

int StringProjectName::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_NAME);
}

bool StringProjectName::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectName *com = static_cast<const StringProjectName*>(other);
    m_newstring = com->m_newstring;
    return true;
}
