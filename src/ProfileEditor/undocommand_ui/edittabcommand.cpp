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
        m_oldid = pxlg.getType(pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex)));
    }

    m_oldauto = pxlg.fetch(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_cache->at(m_targetindex));

    selectIndex(m_newid);

}

void EditTabCommand::undo()
{    
    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_oldauto, m_cache);

    if(m_cache->at(m_targetindex)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
        //temp index
        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_targetindex, QString::number(m_oldid), m_cache);
    }

    setText(QObject::tr("Type change") + QString(" ^(%1)").arg(m_targetindex));
}

void EditTabCommand::redo()
{
    pxlg.replaceElementList(ALL_TYPE, ATTR_ONLY_SCHEDULER, m_targetindex, m_newauto, m_cache);

    if(m_cache->at(m_targetindex)->at(1).at(0) == TE_STACKEDWIDGET_POSITION){
        //temp index
        pxlg.replaceElementList(TE_STACKEDWIDGET_POSITION, ATTR_NONE, m_targetindex, QString::number(m_newid), m_cache);
    }

    setText(QObject::tr("Type change") + QString(" ^(%1)").arg(m_targetindex));
}

int EditTabCommand::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(TE_STACKEDWIDGET_POSITION);
}

void EditTabCommand::selectIndex(int index)
{
    switch (index) {
    case ProcessXmlListGenerator::NORMAL:
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::NORMAL);
        break;
    case ProcessXmlListGenerator::SEARCH:
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::SEARCH);
        break;
    case ProcessXmlListGenerator::EXTRAFUNC:
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::EXTRAFUNC);
        break;
    case ProcessXmlListGenerator::OTHER:
        m_newauto = pxlg.fetch(ALL_TYPE, ATTR_NONE, m_cache->at(m_targetindex), ProcessXmlListGenerator::OTHER);
        break;
    default:
        break;
    }
}
