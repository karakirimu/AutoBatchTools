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

    XmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);

    switch (id()) {
    case XmlListGenerator::SEPARATOR:
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(XmlListGenerator::SEARCH) + 2)
                .at(1);
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(XmlListGenerator::SEARCH) + 3)
                .at(1);
        break;
    case XmlListGenerator::INFO_NAME:
        m_oldstring = m_cache->at(m_targetindex)->at(1).at(1);
        break;
    case XmlListGenerator::INFO_VER:
         m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);
        break;
    case XmlListGenerator::INFO_AUTHOR:
         m_oldstring = m_cache->at(m_targetindex)->at(3).at(1);
        break;
    case XmlListGenerator::INFO_DESCRIPT:
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
    case XmlListGenerator::SEPARATOR:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 3);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 3, alist);
        setText(QObject::tr("Change text at Search Output file"));
        break;
    case XmlListGenerator::INFO_NAME:
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));
        break;
    case XmlListGenerator::INFO_VER:
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));
        break;
    case XmlListGenerator::INFO_AUTHOR:
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));
        break;
    case XmlListGenerator::INFO_DESCRIPT:
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
    case XmlListGenerator::SEPARATOR:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));
        break;
    case XmlListGenerator::OUTPUT_RADIO:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(XmlListGenerator::SEARCH) + 3);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(XmlListGenerator::SEARCH) + 3, alist);
        setText(QObject::tr("Change text at Search Output file"));
        break;
    case XmlListGenerator::INFO_NAME:
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));
        break;
    case XmlListGenerator::INFO_VER:
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));
        break;
    case XmlListGenerator::INFO_AUTHOR:
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));
        break;
    case XmlListGenerator::INFO_DESCRIPT:
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
        return XmlListGenerator::SEPARATOR;
    }else if(m_objname == "outputLineEdit"){
        return XmlListGenerator::OUTPUT_RADIO;
    }else if(m_objname == "nameLineEdit"){
        return XmlListGenerator::INFO_NAME;
    }else if(m_objname == "verLineEdit"){
        return XmlListGenerator::INFO_VER;
    }else if(m_objname == "authorLineEdit"){
        return XmlListGenerator::INFO_AUTHOR;
    }else if(m_objname == "descTextEdit"){
        return XmlListGenerator::INFO_DESCRIPT;
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
