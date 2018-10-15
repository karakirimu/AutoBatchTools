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

//    ProcessXmlListGenerator x;
//    x.getListStructure(cache->at(m_targetindex), &posinfo);

//    m_oldstring = m_cache->at(m_targetindex)
//                ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1)
//                .at(1);
//    m_oldfile = m_cache->at(m_targetindex)
//                ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2)
//                .at(1);

    m_oldstring = pxlg.fetch(PR_NAME, ATTR_NONE, m_cache->at(m_targetindex));
    m_oldfile = pxlg.fetch(PR_FILEPATH, ATTR_NONE, m_cache->at(m_targetindex));

}

void ComboProfileSelect::undo()
{
//    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
//    list1.replace(1, m_oldstring);
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

//    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
//    list2.replace(1, m_oldfile);
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);
    pxlg.replaceElementList(PR_NAME, ATTR_NONE, m_targetindex, m_oldstring, m_cache);
    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, m_targetindex, m_oldfile, m_cache);

    setText(QObject::tr("Other profile to %1 ").arg(m_oldstring) \
            + QString("^(%1)").arg(m_targetindex));
}

void ComboProfileSelect::redo()
{
//    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
//    list1.replace(1, m_newstring);
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

//    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
//    list2.replace(1, m_newfile);
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);
    pxlg.replaceElementList(PR_NAME, ATTR_NONE, m_targetindex, m_newstring, m_cache);
    pxlg.replaceElementList(PR_FILEPATH, ATTR_NONE, m_targetindex, m_newfile, m_cache);

    setText(QObject::tr("Other profile to %1 ").arg(m_newstring) \
            + QString("^(%1)").arg(m_targetindex));
}

int ComboProfileSelect::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(PR_NAME);
}

bool ComboProfileSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboProfileSelect *com = static_cast<const ComboProfileSelect*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
