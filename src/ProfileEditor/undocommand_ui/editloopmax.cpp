#include "editloopmax.h"

EditLoopMax::EditLoopMax(const int &targetindex
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

//    m_oldvalue = static_cast<QString>(m_cache->at(m_targetindex)->at(8).at(3)).toInt();
    m_oldvalue = static_cast<QString>(pxlg.fetch(I_RECURSIVE_LOOP, ATTR_MAXCOUNT, m_cache->at(m_targetindex))).toInt();
}

void EditLoopMax::undo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(8);
//    alist.replace(3, QString::number(m_oldvalue));
//    m_cache->at(m_targetindex)->replace(8, alist);
    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_MAXCOUNT, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Loop Max value to %1 ").arg(m_oldvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

void EditLoopMax::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(8);
//    alist.replace(3, QString::number(m_newvalue));
//    m_cache->at(m_targetindex)->replace(8, alist);
    pxlg.replaceElementList(I_RECURSIVE_LOOP, ATTR_MAXCOUNT, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Loop Max value to %1 ").arg(m_newvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

int EditLoopMax::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_RECURSIVE_LOOP);
}

bool EditLoopMax::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditLoopMax *com = static_cast<const EditLoopMax *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
