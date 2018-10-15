#include "checkeditsearchinput.h"

CheckEditSearchInput::CheckEditSearchInput(const int &targetindex
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

    m_oldcheck = pxlg.fetch(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditSearchInput::undo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
//    alist.replace(SUBPOS, m_oldcheck);

//    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    pxlg.replaceElementList(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Search input %1 ").arg(m_oldcheck) \
            + QString("^(%1)").arg(m_targetindex));
}

void CheckEditSearchInput::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
//    alist.replace(SUBPOS, m_newcheck);

//    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    pxlg.replaceElementList(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Search input %1 ").arg(m_newcheck) \
            + QString("^(%1)").arg(m_targetindex));
}

int CheckEditSearchInput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_FILEINPUT_SEARCHCHECK);
}

//int CheckEditSearchInput::getxmlpos()
//{
//    // match "sinput"
//    return 6;
//}
