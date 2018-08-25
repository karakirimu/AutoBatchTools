#include "combopluginselect.h"

ComboPluginSelect::ComboPluginSelect(const int &targetindex
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
                ->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1)
                .at(1);
    m_oldfile = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2)
                .at(1);
}

void ComboPluginSelect::undo()
{
    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
    list1.replace(1, m_oldstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1, list1);

    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2);
    list2.replace(1, m_oldfile);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2, list2);

    setText(QObject::tr("Plugins to %1").arg(m_oldstring));
}

void ComboPluginSelect::redo()
{
    QStringList list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
    list1.replace(1, m_newstring);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1, list1);

    QStringList list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2);
    list2.replace(1, m_newfile);
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2, list2);

    setText(QObject::tr("Plugins to %1").arg(m_newstring));
}

int ComboPluginSelect::id() const
{
    return ProcessXmlListGenerator::EXTRANAME;
}

bool ComboPluginSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboPluginSelect *com = static_cast<const ComboPluginSelect*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
