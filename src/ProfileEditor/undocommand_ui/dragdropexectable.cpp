#include "dragdropexectable.h"

DragDropExecTable::DragDropExecTable(const int &targetindex
                                     , const QList<int> tablebefore
                                     , const int &tableafter
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_indexbefore = tablebefore;
    m_indexafter = tableafter;

    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(E_CMD, m_cache->at(m_targetindex));

    //sort list assend order
    std::sort(m_indexbefore.begin(), m_indexbefore.end());

    for(int i = 0; i < m_indexbefore.count(); i++){
        m_oldstr.insert(m_indexbefore.at(i)
                        , m_cache->at(m_targetindex)->at(m_indexbefore.at(i) + SKIP).at(1));
    }

}

void DragDropExecTable::undo()
{
    QStringList alist;
    QString sendcode;
    int rcount = -1;

    //delete insert

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int before = 0;
    QString beforedata;
    int sourcecount = m_indexbefore.count();

    for(int i = 0; i < sourcecount; i++){
        before = m_indexbefore.last();

        if(before > m_indexafter){

            //down to up operation
            if(!lastelement){
                lastelement = true;
                deleterow = m_indexafter;
            }

            beforedata = m_oldstr.value(m_indexbefore.at(i));

        }else{

            //up to down operation
            if(!firstelement){
                firstelement = true;
                deleterow = m_indexafter - 1;
            }

            before = m_indexbefore.first();

            beforedata = m_oldstr.value(m_indexbefore.at(sourcecount - 1 - i));
        }

        m_cache->at(m_targetindex)->removeAt(deleterow + SKIP);

        alist = ProcessXmlListGenerator::createExecElement(beforedata, before);
        m_cache->at(m_targetindex)->insert(before + SKIP, alist);
    }

    rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    updateIndex(rcount);

    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(m_targetindex).arg(deleterow) \
                                         .arg(m_indexbefore.count()).arg(before) \
                                         .arg(UNDOREDO_E_TABLEMOVE);
    setText(QObject::tr("Move exec element at %1").arg(QString::number(m_indexafter)) + sendcode);
}

void DragDropExecTable::redo()
{
    QStringList alist;
    QString sendcode;
    int rcount = -1;

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int updown = 0;
    int before = 0;
    QString beforedata;
    int sourcecount = m_indexbefore.count();

    int deductnum = 1;

    //multiple element move
    for(int i = 0; i < sourcecount; i++){
        before = m_indexbefore.at(i);

        if(before > m_indexafter){

            if(!lastelement){
                lastelement = true;
                deleterow = m_indexbefore.last();
            }

            beforedata = m_oldstr.value(m_indexbefore.at(sourcecount - deductnum));
            deductnum++;
            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = m_indexbefore.first();
            }

            beforedata = m_oldstr.value(before);
            updown = -1;

        }

        m_cache->at(m_targetindex)->removeAt(deleterow + SKIP);

        alist = ProcessXmlListGenerator::createExecElement(beforedata, m_indexafter + updown);
        m_cache->at(m_targetindex)->insert(m_indexafter + updown + SKIP, alist);
    }

    rcount = static_cast<QString>(pxlg.fetch(E_CMDARGCOUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    updateIndex(rcount);


    //  "process" target index
    //  exec table first before(source) index,
    //  exec table before(source) contiguous count (incremental),
    //  exec table after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4,%5)").arg(m_targetindex).arg(deleterow) \
                                         .arg(m_indexbefore.count()).arg(m_indexafter) \
                                         .arg(UNDOREDO_E_TABLEMOVE);

    setText(QObject::tr("Move exec element at %1").arg(QString::number(m_indexafter)) + sendcode);
}

void DragDropExecTable::updateIndex(int count)
{
    QStringList alist;
    int minbefore = m_indexbefore.first();
    int minindex = minbefore > m_indexafter ? m_indexafter : minbefore;

    for(int i = minindex; i < count; i++){
        alist = m_cache->at(m_targetindex)->at(i + SKIP);
        alist.replace(3, QString::number(i));
        m_cache->at(m_targetindex)->replace(i + SKIP, alist);

    }
}
