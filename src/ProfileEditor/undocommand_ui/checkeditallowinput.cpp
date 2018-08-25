#include "checkeditallowinput.h"

CheckEditAllowInput::CheckEditAllowInput(const int &targetindex
                               , const bool &newcheck
                               , QList<QList<QStringList> *> *cache
                               , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    if(m_targetindex > 1){
        //init generator
        ProcessXmlListGenerator x;
        x.getListStructure(m_cache->at(m_targetindex), &xmlpos);
    }

    m_oldcheck = m_cache->at(m_targetindex)->at(getxmlpos()).at(SUBPOS);

}

void CheckEditAllowInput::undo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
    alist.replace(SUBPOS, m_oldcheck);

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Allow input %1").arg(m_oldcheck));
}

void CheckEditAllowInput::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(getxmlpos());
    alist.replace(SUBPOS, m_newcheck);

    m_cache->at(m_targetindex)->replace(getxmlpos(), alist);
    setText(QObject::tr("Allow input %1").arg(m_newcheck));
}

int CheckEditAllowInput::getxmlpos()
{
    // match "finput"
    return 5;
}