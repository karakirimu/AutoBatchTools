#include "editcheckcommand.h"

EditCheckCommand::EditCheckCommand(const int &targetindex
                                   , const bool &newcheck
                                   , const QString objname
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_objname = objname;
    m_cache = cache;

    //init generator
    ProcessXmlListGenerator x;
    x.getListStructure(m_cache->at(m_targetindex), &xmlpos);

    if(m_objname == "timeoutCheckBox"){
        m_oldcheck = m_cache->at(m_targetindex)->at(getxmlpos()).at(1);
    }else{
        m_oldcheck = m_cache->at(m_targetindex)->at(getxmlpos()).at(3);
    }
}

void EditCheckCommand::undo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());


    if(m_objname == "timeoutCheckBox"){
        alist.replace(1, m_oldcheck);
//        m_cache->at(m_targetindex)->at(getxmlpos()).replace(1, m_oldcheck);
    }else{
        alist.replace(3, m_oldcheck);
//        m_cache->at(m_targetindex)->at(getxmlpos()).replace(3, m_oldcheck);
    }

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Change CheckBox"));
}

void EditCheckCommand::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());

    if(m_objname == "timeoutCheckBox"){
        alist.replace(1, m_newcheck);
//        m_cache->at(m_targetindex)->at(getxmlpos()).replace(1, m_newcheck);
    }else{
        alist.replace(3, m_newcheck);
//       m_cache->at(m_targetindex)->at(getxmlpos()).replace(3, m_newcheck);
    }

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Change CheckBox"));
}

//DEPENDS_XML DEPENDS_UI
int EditCheckCommand::getxmlpos()
{
    if(m_objname == "timeoutCheckBox"){
        return xmlpos.value(ProcessXmlListGenerator::NORMAL) + 1;

    }else if(m_objname == "autoOnlyCheckBox"){
        return xmlpos.value(ProcessXmlListGenerator::NORMAL);

    }else if(m_objname == "autoOnlyCheckBox_2"){
        return xmlpos.value(ProcessXmlListGenerator::SEARCH);

    }else if(m_objname == "autoOnlyCheckBox_3"){
        return xmlpos.value(ProcessXmlListGenerator::EXTRAFUNC);

    }else if(m_objname == "autoOnlyCheckBox_4"){
        return xmlpos.value(ProcessXmlListGenerator::OTHER);

    }else{
        return -1;
    }
}
