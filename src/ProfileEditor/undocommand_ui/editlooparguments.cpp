#include "editlooparguments.h"

EditLoopArguments::EditLoopArguments(const int &targetindex
                                     , int newvalue
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

//    if(m_targetindex > 1){
//        //init generator
//        ProcessXmlListGenerator x;
//        x.getListStructure(m_cache->at(m_targetindex), &posinfo);
//    }

//    m_oldvalue = static_cast<QString>(m_cache->at(m_targetindex)->at(9).at(1)).toInt();
    m_oldvalue = static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
}

void EditLoopArguments::undo()
{
//    QStringList alist;
//    alist = m_cache->at(m_targetindex)->at(9);
//    alist.replace(1, QString::number(m_oldvalue));
//    m_cache->at(m_targetindex)->replace(9, alist);
    pxlg.replaceElementList(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Arguments count to %1 ").arg(m_oldvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

void EditLoopArguments::redo()
{
//    QStringList alist;
//    alist = m_cache->at(m_targetindex)->at(9);
//    alist.replace(1, QString::number(m_newvalue));
//    m_cache->at(m_targetindex)->replace(9, alist);
    pxlg.replaceElementList(I_RECURSIVE_LOOPARGCOUNT, ATTR_NONE, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Arguments count to %1 ").arg(m_newvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

int EditLoopArguments::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_RECURSIVE_LOOPARGCOUNT);
}

bool EditLoopArguments::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopArguments *com = static_cast<const EditLoopArguments *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
