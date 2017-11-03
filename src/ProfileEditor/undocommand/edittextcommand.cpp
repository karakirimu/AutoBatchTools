#include "edittextcommand.h"

EditTextCommand::EditTextCommand(const int &targetindex
                                 , QString newstring
                                 , const QString &objname
                                 , QList<QList<QStringList> *> *cache
                                 , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_objname = objname;
    m_cache = cache;

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    switch (id()) {
    case ProcessXmlListGenerator::SEPARATOR:
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2)
                .at(1);
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 3)
                .at(1);
        break;
    case ProcessXmlListGenerator::INFO_NAME:
        m_oldstring = m_cache->at(m_targetindex)->at(1).at(1);
        break;
    case ProcessXmlListGenerator::INFO_VER:
         m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);
        break;
    case ProcessXmlListGenerator::INFO_AUTHOR:
         m_oldstring = m_cache->at(m_targetindex)->at(3).at(1);
        break;
    case ProcessXmlListGenerator::INFO_DESCRIPT:
         m_oldstring = m_cache->at(m_targetindex)->at(4).at(1);
        break;
    default:
        break;
    }
}

void EditTextCommand::undo()
{
    if(m_cache->isEmpty()) return;

    QStringList alist;

    switch (id()) {
    case ProcessXmlListGenerator::SEPARATOR:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 3);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 3, alist);
        setText(QObject::tr("Change text at Search Output file"));
        break;
    case ProcessXmlListGenerator::INFO_NAME:
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));
        break;
    case ProcessXmlListGenerator::INFO_VER:
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));
        break;
    case ProcessXmlListGenerator::INFO_AUTHOR:
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));
        break;
    case ProcessXmlListGenerator::INFO_DESCRIPT:
        alist = m_cache->at(m_targetindex)->at(4);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(4, alist);
        setText(QObject::tr("Change description"));
        break;
    default:
        break;
    }
}

void EditTextCommand::redo()
{
    QStringList alist;

    switch (id()) {
    case ProcessXmlListGenerator::SEPARATOR:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));
        break;
    case ProcessXmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 3);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 3, alist);
        setText(QObject::tr("Change text at Search Output file"));
        break;
    case ProcessXmlListGenerator::INFO_NAME:
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));
        break;
    case ProcessXmlListGenerator::INFO_VER:
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));
        break;
    case ProcessXmlListGenerator::INFO_AUTHOR:
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));
        break;
    case ProcessXmlListGenerator::INFO_DESCRIPT:
        alist = m_cache->at(m_targetindex)->at(4);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(4, alist);
        setText(QObject::tr("Change description"));
        break;
    default:
        break;
    }
}

//DEPENDS_XML DEPENDS_UI
int EditTextCommand::id() const
{
    if(m_objname == "separatorLineEdit"){
        return ProcessXmlListGenerator::SEPARATOR;
    }else if(m_objname == "outputLineEdit"){
        return ProcessXmlListGenerator::OUTPUT_RADIO;
    }else if(m_objname == "nameLineEdit"){
        return ProcessXmlListGenerator::INFO_NAME;
    }else if(m_objname == "verLineEdit"){
        return ProcessXmlListGenerator::INFO_VER;
    }else if(m_objname == "authorLineEdit"){
        return ProcessXmlListGenerator::INFO_AUTHOR;
    }else if(m_objname == "descTextEdit"){
        return ProcessXmlListGenerator::INFO_DESCRIPT;
    }else {
        return -1;
    }
}

bool EditTextCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    m_newstring = static_cast<const EditTextCommand*>(other)->m_newstring;
    return true;
}
