#include "editscomboboxcommand.h"

EditScomboBoxCommand::EditScomboBoxCommand(const int &targetindex
                                           , const QString &newstring
                                           , const int &newsearchindex
                                           , QList<QList<QStringList> *> *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    m_newindex = newsearchindex;

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1)
                .at(1);
    m_oldindex = ((QString)(m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1)
                .at(3))).toInt();
}

void EditScomboBoxCommand::undo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1);
    list.replace(1, m_oldstring);
    list.replace(3, QString::number(m_oldindex));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1, list);

    setText(QObject::tr("Change Search Function to ") + m_oldstring);
}

void EditScomboBoxCommand::redo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1);
    list.replace(1, m_newstring);
    list.replace(3, QString::number(m_newindex));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 1, list);

    setText(QObject::tr("Change Search Function to ") + m_newstring);
}
