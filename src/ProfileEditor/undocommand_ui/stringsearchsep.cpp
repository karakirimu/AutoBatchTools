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

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)
            ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2)
            .at(1);
}

void StringSearchSep::undo()
{
//    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
    alist.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
    setText(QObject::tr("Change text at Search Separator"));
}

void StringSearchSep::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
    alist.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
    setText(QObject::tr("Change text at Search Separator"));
}

int StringSearchSep::id() const
{
    return ProcessXmlListGenerator::SEPARATOR;
}

bool StringSearchSep::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringSearchSep *com = static_cast<const StringSearchSep*>(other);
    m_newstring = com->m_newstring;
    return true;
}
