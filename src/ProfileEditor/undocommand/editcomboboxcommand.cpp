#include "editcomboboxcommand.h"

EditComboBoxCommand::EditComboBoxCommand(const int &targetindex
                                         , const QString newstring
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    XmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(XmlListGenerator::SEARCH) + 3)
                .at(1);

}

void EditComboBoxCommand::undo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 3);
    list.replace(1, m_oldstring);

    m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 3, list);
    setText(QObject::tr("Change return variant to %1").arg(m_oldstring));
}

void EditComboBoxCommand::redo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 3);
    list.replace(1, m_newstring);

    m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 3, list);
    setText(QObject::tr("Change return variant to %1").arg(m_newstring));
}
