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
    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)->at(4).at(1);
}

void StringProjectDescription::undo()
{
//    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(4);
    alist.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(4, alist);
    setText(QObject::tr("Description Text"));
}

void StringProjectDescription::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(4);
    alist.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(4, alist);
    setText(QObject::tr("Description Text"));
}

int StringProjectDescription::id() const
{
    return ProcessXmlListGenerator::INFO_DESCRIPT;
}

bool StringProjectDescription::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectDescription *com = static_cast<const StringProjectDescription*>(other);
    m_newstring = com->m_newstring;
    return true;
}
