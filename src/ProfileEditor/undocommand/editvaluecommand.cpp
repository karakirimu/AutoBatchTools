#include "editvaluecommand.h"

EditValueCommand::EditValueCommand(const int &targetindex
                                   , int newvalue
                                   , const QString &objname
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_objname = objname;
    m_cache = cache;

    //init generator
    XmlListGenerator x;
    x.getListStructure(m_cache->at(m_targetindex), &posinfo);

    //get old value
    switch (id()) {
    case XmlListGenerator::TIMEOUT_DURITION:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)
                ->at(posinfo.value(XmlListGenerator::NORMAL) + 1)
                .at(3)).toInt();
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)
                ->at(posinfo.value(XmlListGenerator::SEARCH) + 4)
                .at(3)).toInt();
        break;
    default:
        break;
    }
}

void EditValueCommand::undo()
{
    QStringList alist;
    switch (id()) {
    case XmlListGenerator::TIMEOUT_DURITION:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::NORMAL) + 1);
        alist.replace(3, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::NORMAL) + 1, alist);

        setText(QObject::tr("Change timeout value %1").arg(m_oldvalue));
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 4);
        alist.replace(3, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 4, alist);

        setText(QObject::tr("Change output method"));
        break;
    default:
        break;
    }
}

void EditValueCommand::redo()
{
    QStringList alist;
    switch (id()) {
    case XmlListGenerator::TIMEOUT_DURITION:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::NORMAL) + 1);
        alist.replace(3, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::NORMAL) + 1, alist);

        setText(QObject::tr("Change timeout value %1").arg(m_newvalue));
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 4);
        alist.replace(3, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 4, alist);

        setText(QObject::tr("Change output method"));
        break;
    default:
        break;
    }
}

//DEPENDS_XML DEPENDS_UI
int EditValueCommand::id() const
{
    if(m_objname == "timeoutSpinBox"){
        return XmlListGenerator::TIMEOUT_DURITION;
    }else if(m_objname == "variRadioButton"
             || m_objname == "fileRadioButton"){
        return XmlListGenerator::OUTPUT_RADIO;
    }else{
        return -1;
    }
}

bool EditValueCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditValueCommand *com = static_cast<const EditValueCommand *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
