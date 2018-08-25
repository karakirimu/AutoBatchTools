#include "comboprofileselect.h"

ComboProfileSelect::ComboProfileSelect(const int &targetindex
                                       , const QString newstring
                                       , const QString newfile
                                       , QList<QList<QStringList> *> *cache
                                       , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    m_newfile = newfile;

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1)
                .at(1);
    m_oldfile = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2)
                .at(1);

}

void ComboProfileSelect::undo()
{
    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
    list1.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
    list2.replace(1, m_oldfile);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);

    setText(QObject::tr("Other profile to %1").arg(m_oldstring));
}

void ComboProfileSelect::redo()
{
    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
    list1.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
    list2.replace(1, m_newfile);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);

    setText(QObject::tr("Other profile to %1").arg(m_newstring));
}

int ComboProfileSelect::id() const
{
    return ProcessXmlListGenerator::OTHERNAME;
}

bool ComboProfileSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboProfileSelect *com = static_cast<const ComboProfileSelect*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
