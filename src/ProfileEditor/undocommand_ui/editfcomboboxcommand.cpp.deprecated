#include "editfcomboboxcommand.h"

EditFComboBoxCommand::EditFComboBoxCommand(const int &targetindex
//                                           , const QString oldstring
                                           , const QString newstring
//                                           , const QString oldfile
                                           , const QString newfile
                                           , const QString &objname
                                           , QList<QList<QStringList> *> *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_objname = objname;
    m_cache = cache;

    m_newfile = newfile;

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    switch (id()) {
    case ProcessXmlListGenerator::EXTRANAME:
        m_oldstring = m_cache->at(m_targetindex)
                    ->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1)
                    .at(1);
        m_oldfile = m_cache->at(m_targetindex)
                    ->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2)
                    .at(1);
        break;
    case ProcessXmlListGenerator::OTHERNAME:
        m_oldstring = m_cache->at(m_targetindex)
                    ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1)
                    .at(1);
        m_oldfile = m_cache->at(m_targetindex)
                    ->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2)
                    .at(1);
        break;
    default:
        break;
    }

}

void EditFComboBoxCommand::undo()
{
    QStringList list1;
    QStringList list2;

    switch (id()) {
    case ProcessXmlListGenerator::EXTRANAME:
        list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
        list1.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1, list1);

        list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2);
        list2.replace(1, m_oldfile);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2, list2);

        setText(QObject::tr("Change Extra to %1").arg(m_oldstring));
        break;
    case ProcessXmlListGenerator::OTHERNAME:
        list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
        list1.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

        list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
        list2.replace(1, m_oldfile);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);

        setText(QObject::tr("Change other profile to %1").arg(m_oldstring));
        break;
    default:
        break;
    }
}

void EditFComboBoxCommand::redo()
{
    QStringList list1;
    QStringList list2;

    switch (id()) {
    case ProcessXmlListGenerator::EXTRANAME:
        list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1);
        list1.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 1, list1);

        list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2);
        list2.replace(1, m_newfile);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 2, list2);

        setText(QObject::tr("Change Extra to %1").arg(m_newstring));
        break;
    case ProcessXmlListGenerator::OTHERNAME:
        list1 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 1);
        list1.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 1, list1);

        list2 = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::OTHER) + 2);
        list2.replace(1, m_newfile);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::OTHER) + 2, list2);

        setText(QObject::tr("Change other profile to %1").arg(m_newstring));
        break;
    default:
        break;
    }
}

//DEPENDS_XML DEPENDS_UI
int EditFComboBoxCommand::id() const
{
    if(m_objname == "extrafuncComboBox"){
        return ProcessXmlListGenerator::EXTRANAME;
    }else if(m_objname == "profileComboBox"){
        return ProcessXmlListGenerator::OTHERNAME;
    }else{
        return -1;
    }
}

bool EditFComboBoxCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditFComboBoxCommand *com = static_cast<const EditFComboBoxCommand*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
