#include "edittabcommand.h"

EditTabCommand::EditTabCommand(const int &targetindex
                               , const int &newid
                               , QList<QList<QStringList>* > *cache
                               , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newid = newid;
    m_cache = cache;

    if(m_cache->at(m_targetindex)->at(1).at(0) == "istack"){
        //temp index
        m_oldid = ((QString)m_cache->at(m_targetindex)->at(1).at(1)).toInt();
    }else{
        //no istack
        ProcessXmlListGenerator x;
        m_oldid = x.getType(m_cache->at(m_targetindex)->at(0).at(1));
    }

    m_oldauto = m_cache->at(m_targetindex)->at(0).at(3);

    selectIndex(m_newid);

}

void EditTabCommand::undo()
{    
    QStringList alist = m_cache->at(m_targetindex)->at(0);
    alist.replace(3, m_oldauto);
    m_cache->at(m_targetindex)->replace(0, alist);

    if(m_cache->at(m_targetindex)->at(1).at(0) == "istack"){
        //temp index
        QStringList list = m_cache->at(m_targetindex)->at(1);
        list.replace(1, QString::number(m_oldid));
        m_cache->at(m_targetindex)->replace(1, list);
    }

    setText(QString("Edit type change at %1").arg(m_targetindex));
}

void EditTabCommand::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(0);
    alist.replace(3, m_newauto);
    m_cache->at(m_targetindex)->replace(0, alist);

    if(m_cache->at(m_targetindex)->at(1).at(0) == "istack"){
        //temp index
        QStringList list = m_cache->at(m_targetindex)->at(1);
        list.replace(1, QString::number(m_newid));
        m_cache->at(m_targetindex)->replace(1, list);
    }

    setText(QString("Edit type change at %1").arg(m_targetindex));
}

void EditTabCommand::selectIndex(int index)
{
    ProcessXmlListGenerator x;
    QHash<int, int> data;
    x.getListStructure(m_cache->at(m_targetindex), &data);
    switch (index) {
    case ProcessXmlListGenerator::NORMAL:
        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::NORMAL)).at(3);
        break;
    case ProcessXmlListGenerator::SEARCH:
        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::SEARCH)).at(3);
        break;
    case ProcessXmlListGenerator::EXTRAFUNC:
        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::EXTRAFUNC)).at(3);
        break;
    case ProcessXmlListGenerator::OTHER:
        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::OTHER)).at(3);
        break;
    default:
        break;
    }
}
