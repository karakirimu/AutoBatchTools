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

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)->at(1).at(1);
}

void StringProjectName::undo()
{
//    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(1);
    alist.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(1, alist);
    setText(QObject::tr("Project name changed"));
}

void StringProjectName::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(1);
    alist.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(1, alist);
    setText(QObject::tr("Project name changed"));
}

int StringProjectName::id() const
{
    return ProcessXmlListGenerator::INFO_NAME;
}

bool StringProjectName::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectName *com = static_cast<const StringProjectName*>(other);
    m_newstring = com->m_newstring;
    return true;
}
