#include "edittimeout.h"

EditTimeout::EditTimeout(const int &targetindex
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

    //get old value
//    m_oldvalue = static_cast<QString>(m_cache->at(m_targetindex)
//                ->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1)
//                .at(3)).toInt();
    m_oldvalue = static_cast<QString>(pxlg.fetch(E_TIMEOUT, ATTR_TIMEOUTMS, m_cache->at(m_targetindex))).toInt();
}

void EditTimeout::undo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
//    alist.replace(3, QString::number(m_oldvalue));
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);
    pxlg.replaceElementList(E_TIMEOUT, ATTR_TIMEOUTMS, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Timeout value to %1").arg(m_oldvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

void EditTimeout::redo()
{
//    QStringList alist;
//    alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1);
//    alist.replace(3, QString::number(m_newvalue));
//    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL) + 1, alist);

    pxlg.replaceElementList(E_TIMEOUT, ATTR_TIMEOUTMS, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Timeout value to %1 ").arg(m_newvalue) \
            + QString("^(%1)").arg(m_targetindex));
}

int EditTimeout::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(E_TIMEOUT);
}

bool EditTimeout::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTimeout *com = static_cast<const EditTimeout *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
