#include "dragdropcommand.h"

DragDropCommand::DragDropCommand(const QList<int> &beforeindex
                                 , const int &afterindex
                                 , QList<QList<QStringList> *> *cache
                                 , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_beforeindex = beforeindex;
    m_afterindex = afterindex;
    m_cache = cache;
}

void DragDropCommand::undo()
{
    QString sendcode;

    //delete insert
    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int before = 0;
    int deductnum = 1;

    QList<QStringList> *beforedata;
    int sourcecount = m_beforeindex.count();

    for(int i = 0; i < sourcecount; i++){
        before = m_beforeindex.last();

        if(before > m_afterindex){

            //down to up operation
            if(!lastelement){
                lastelement = true;
                deleterow = m_afterindex;
            }

            beforedata = m_cache->at(deleterow);

        }else{

            //up to down operation
            if(!firstelement){
                firstelement = true;
                deleterow = m_afterindex - 1;
            }

            before = m_beforeindex.first();

            beforedata = m_cache->at(deleterow);
            deductnum++;

        }

        m_cache->removeAt(deleterow);
        m_cache->insert(before, beforedata);
    }

    //  "process" first before(source) index,
    //  "process" before(source) contiguous count (incremental),
    //  "process" after(target) index,
    //  sendcode
    sendcode = QString(" ^(%1,%2,%3,%4)").arg(deleterow) \
                                         .arg(m_beforeindex.count()).arg(before) \
                                         .arg(UNDOREDO_MOVE);
    setText(QObject::tr("Move item at %1").arg(QString::number(m_afterindex)) + sendcode);
}

void DragDropCommand::redo()
{
    QString sendcode;

    int deleterow = 0;
    bool firstelement = false;
    bool lastelement = false;

    int updown = 0;
    int before = 0;

    QList<QStringList> *beforedata;
    int sourcecount = m_beforeindex.count();

    //multiple element move
    for(int i = 0; i < sourcecount; i++){
        before = m_beforeindex.at(i);

        if(before > m_afterindex){

            if(!lastelement){
                lastelement = true;
                deleterow = m_beforeindex.last();
            }

            beforedata = m_cache->at(deleterow);
            updown = 0;

        }else{

            if(!firstelement){
                firstelement = true;
                deleterow = m_beforeindex.first();
            }

            beforedata = m_cache->at(deleterow);
            updown = -1;

        }

        m_cache->removeAt(deleterow);
        m_cache->insert(m_afterindex + updown, beforedata);
    }

    //  "process" first before(source) index,
    //  "process" before(source) contiguous count (incremental),
    //  "process" after(target) index, sendcode
    sendcode = QString(" ^(%1,%2,%3,%4)").arg(deleterow) \
                                         .arg(m_beforeindex.count()).arg(m_afterindex) \
                                         .arg(UNDOREDO_MOVE);

    setText(QObject::tr("Move item at %1").arg(QString::number(m_afterindex)) + sendcode);

}
