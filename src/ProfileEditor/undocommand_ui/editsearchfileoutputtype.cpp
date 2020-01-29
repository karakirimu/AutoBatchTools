#include "editsearchfileoutputtype.h"

EditSearchFileOutputType::EditSearchFileOutputType(const int &targetindex
                                                   , int newvalue
                                                   , QList<QList<QStringList> *> *cache
                                                   , QUndoCommand *parent)
                    :QUndoCommand(parent)
                {
                    m_targetindex = targetindex;
                    m_newvalue = newvalue;
                    m_cache = cache;

                    m_oldvalue = static_cast<QString>(pxlg.fetch(S_OUTPUTFILETYPE, ATTR_NONE, m_cache->at(m_targetindex))).toInt();

                }

void EditSearchFileOutputType::undo()
{
    if(m_cache->isEmpty()) return;

    pxlg.replaceElementList(S_OUTPUTFILETYPE, ATTR_NONE, m_targetindex, QString::number(m_oldvalue), m_cache);

    setText(QObject::tr("Change output file method") \
            + QString(" ^(%1)").arg(m_targetindex));
}

void EditSearchFileOutputType::redo()
{
    pxlg.replaceElementList(S_OUTPUTFILETYPE, ATTR_NONE, m_targetindex, QString::number(m_newvalue), m_cache);

    setText(QObject::tr("Change output file method") \
            + QString(" ^(%1)").arg(m_targetindex));
}

int EditSearchFileOutputType::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(ATTR_NONE);
}

bool EditSearchFileOutputType::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchFileOutputType *com = static_cast<const EditSearchFileOutputType *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
