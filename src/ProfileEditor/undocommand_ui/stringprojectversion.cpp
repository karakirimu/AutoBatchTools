#include "stringprojectversion.h"

StringProjectVersion::StringProjectVersion(const int &targetindex
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

    //info version
    m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);

}

void StringProjectVersion::undo()
{
//    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(2);
    alist.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(2, alist);
    setText(QObject::tr("Version"));
}

void StringProjectVersion::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(2);
    alist.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(2, alist);
    setText(QObject::tr("Version"));
}

int StringProjectVersion::id() const
{
    return ProcessXmlListGenerator::INFO_VER;
}

bool StringProjectVersion::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectVersion *com = static_cast<const StringProjectVersion*>(other);
    m_newstring = com->m_newstring;
    return true;
}
