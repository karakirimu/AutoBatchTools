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

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)
            ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2)
            .at(1);
}

void EditSearchTextSep::undo()
{
//    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
    alist.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
    setText(QObject::tr("Change text at Search Separator"));
}

void EditSearchTextSep::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
    alist.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
    setText(QObject::tr("Change text at Search Separator"));
}

int EditSearchTextSep::id() const
{
    return ProcessXmlListGenerator::SEPARATOR;
}

bool EditSearchTextSep::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchTextSep *com = static_cast<const EditSearchTextSep*>(other);
    m_newstring = com->m_newstring;
    return true;
}
