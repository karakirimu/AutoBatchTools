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

    if(m_cache->at(m_targetindex)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
        //temp index
        m_oldid = static_cast<QString>(pxlg.fetch(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    }else{
        //no istack
//        ProcessXmlListGenerator x;
//        m_oldid = x.getType(m_cache->at(m_targetindex)->at(0).at(1));

        m_oldid = pxlg.getType(pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex)));
    }

//    m_oldauto = m_cache->at(m_targetindex)->at(0).at(3);
    m_oldauto = pxlg.fetch(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));

    selectIndex(m_newid);

}

void EditTabCommand::undo()
{    
//    QStringList alist = m_cache->at(m_targetindex)->at(0);
//    alist.replace(3, m_oldauto);
//    m_cache->at(m_targetindex)->replace(0, alist);

    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldauto, m_cache);

    if(m_cache->at(m_targetindex)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
        //temp index
//        QStringList list = m_cache->at(m_targetindex)->at(1);
//        list.replace(1, QString::number(m_oldid));
//        m_cache->at(m_targetindex)->replace(1, list);
        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_targetindex, QString::number(m_oldid), m_cache);
    }

    setText(QObject::tr("Type change ") + QString("^(%1)").arg(m_targetindex));
}

void EditTabCommand::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(0);
//    alist.replace(3, m_newauto);
//    m_cache->at(m_targetindex)->replace(0, alist);

    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_newauto, m_cache);

    if(m_cache->at(m_targetindex)->at(1).at(0) == "istack"){
        //temp index
//        QStringList list = m_cache->at(m_targetindex)->at(1);
//        list.replace(1, QString::number(m_newid));
//        m_cache->at(m_targetindex)->replace(1, list);
        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_targetindex, QString::number(m_newid), m_cache);
    }

    setText(QObject::tr("Type change ") + QString("^(%1)").arg(m_targetindex));
}

int EditTabCommand::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(TE_STACKEDWIDGET_POSITION);
}

void EditTabCommand::selectIndex(int index)
{
//    ProcessXmlListGenerator x;
//    QHash<int, int> data;
//    x.getListStructure(m_cache->at(m_targetindex), &data);
    switch (index) {
    case ProcessXmlListGenerator::NORMAL:
//        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::NORMAL)).at(3);
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::NORMAL);
        break;
    case ProcessXmlListGenerator::SEARCH:
//        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::SEARCH)).at(3);
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::SEARCH);
        break;
    case ProcessXmlListGenerator::EXTRAFUNC:
//        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::EXTRAFUNC)).at(3);
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::EXTRAFUNC);
        break;
    case ProcessXmlListGenerator::OTHER:
//        m_newauto = m_cache->at(m_targetindex)->at(data.value(ProcessXmlListGenerator::OTHER)).at(3);
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::OTHER);
        break;
    default:
        break;
    }
}
