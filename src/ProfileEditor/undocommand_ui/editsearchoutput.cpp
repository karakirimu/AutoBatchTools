#include "editsearchoutput.h"

EditSearchOutput::EditSearchOutput(const int &targetindex
                                   , int newvalue
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    m_oldvalue = static_cast<QString>(pxlg.fetch(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, m_cache->at(m_targetindex))).toInt();

}

void EditSearchOutput::undo()
{
    if(m_cache->isEmpty()) return;

    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Output Select") \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void EditSearchOutput::redo()
{
    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_RADIOBUTTONPOS, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Output Select") \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int EditSearchOutput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(ATTR_RADIOBUTTONPOS);
}

bool EditSearchOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchOutput *com = static_cast<const EditSearchOutput *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
