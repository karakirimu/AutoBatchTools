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

    if(m_targetindex > 1){
        //init generator
        ProcessXmlListGenerator x;
        x.getListStructure(m_cache->at(m_targetindex), &posinfo);
    }

    //get old value
    switch (id()) {
    case ProcessXmlListGenerator::getId(E_TIMEOUT):
        m_oldvalue = ((QString)m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1)
                .at(3)).toInt();
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4)
                .at(3)).toInt();
        break;

    case ProcessXmlListGenerator::INFO_RLOOP:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(8).at(3)).toInt();
        break;

    case ProcessXmlListGenerator::INFO_RLARG:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(9).at(1)).toInt();
        break;

    case ProcessXmlListGenerator::INFO_RELOOP:
        m_oldvalue = ((QString)m_cache->at(m_targetindex)->at(10).at(1)).toInt();
        break;

    default:
        break;
    }
}

void EditValueCommand::undo()
{
    QStringList alist;
    switch (id()) {
    case ProcessXmlListGenerator::getId(E_TIMEOUT):
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
        alist.replace(3, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);

        setText(QObject::tr("Timeout value to %1").arg(m_oldvalue));
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
        alist.replace(3, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);

        setText(QObject::tr("Change output method"));
        break;
    case ProcessXmlListGenerator::INFO_RLOOP:
        alist = m_cache->at(m_targetindex)->at(8);
        alist.replace(3, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(8, alist);

        setText(QObject::tr("Loop Max value to %1").arg(m_oldvalue));
        break;
    case ProcessXmlListGenerator::INFO_RLARG:
        alist = m_cache->at(m_targetindex)->at(9);
        alist.replace(1, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(9, alist);

        setText(QObject::tr("Arguments count to %1").arg(m_oldvalue));
        break;
    case ProcessXmlListGenerator::INFO_RELOOP:
        alist = m_cache->at(m_targetindex)->at(10);
        alist.replace(1, QString::number(m_oldvalue));
        m_cache->at(m_targetindex)->replace(10, alist);

        setText(QObject::tr("Loop recursive value to %1").arg(m_oldvalue));
        break;
    default:
        break;
    }
}

void EditValueCommand::redo()
{
    QStringList alist;
    switch (id()) {
    case ProcessXmlListGenerator::getId(E_TIMEOUT):
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
        alist.replace(3, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);

        setText(QObject::tr("Timeout value to %1").arg(m_newvalue));
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
        alist.replace(3, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);

        setText(QObject::tr("Change output method"));
        break;
    case ProcessXmlListGenerator::INFO_RLOOP:
        alist = m_cache->at(m_targetindex)->at(8);
        alist.replace(3, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(8, alist);

        setText(QObject::tr("Loop Max value to %1").arg(m_newvalue));
        break;
    case ProcessXmlListGenerator::INFO_RLARG:
        alist = m_cache->at(m_targetindex)->at(9);
        alist.replace(1, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(9, alist);

        setText(QObject::tr("Arguments count to %1").arg(m_newvalue));
        break;
    case ProcessXmlListGenerator::INFO_RELOOP:
        alist = m_cache->at(m_targetindex)->at(10);
        alist.replace(1, QString::number(m_newvalue));
        m_cache->at(m_targetindex)->replace(10, alist);

        setText(QObject::tr("Loop recursive value to %1").arg(m_newvalue));
        break;
    default:
        break;
    }
}

//DEPENDS_XML DEPENDS_UI
int EditValueCommand::id() const
{
    if(m_objname == "timeoutSpinBox"){
        return ProcessXmlListGenerator::getId(E_TIMEOUT);

    }else if(m_objname == "variRadioButton"
             || m_objname == "fileRadioButton"){
        return ProcessXmlListGenerator::getId(ATTR_RADIOBUTTONPOS);

    }else if(m_objname == "loopMaxSpinBox"){
        return ProcessXmlListGenerator::getId(I_RECURSIVE_LOOP);

    }else if(m_objname == "loopArgumentsSpinBox"){
        return ProcessXmlListGenerator::getId(I_RECURSIVE_LOOPARGCOUNT);

    }else if(m_objname == "loopRecursiveSpinBox"){
        return ProcessXmlListGenerator::getId(I_RECURSIVE_LOOPCOUNT);
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
