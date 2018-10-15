#include "checkedittimeout.h"

CheckEditTimeout::CheckEditTimeout(const int &targetindex
                                         , const bool &newcheck
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

//    if(m_targetindex > 1){
//        //init generator
//        ProcessXmlListGenerator x;
//        x.getListStructure(m_cache->at(m_targetindex), &xmlpos);
//    }

//    m_oldcheck = m_cache->at(m_targetindex)->at(getxmlpos()).at(SUBPOS);
    m_oldcheck = pxlg.fetch(E_TIMEOUT, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditTimeout::undo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
//    alist.replace(1, m_oldcheck);

//    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Timeout %1 ").arg(m_oldcheck) \
            + QString("^(%1)").arg(m_targetindex));
}

void CheckEditTimeout::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
//    alist.replace(1, m_newcheck);

//    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    pxlg.replaceElementList(E_TIMEOUT, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Timeout %1 ").arg(m_newcheck) \
            + QString("^(%1)").arg(m_targetindex));
}

//int CheckEditTimeout::getxmlpos()
//{
//    return xmlpos.value(ProcessXmlListGenerator::NORMAL) + 1;
//}
