#include "stringfileoutput.h"

StringFileOutput::StringFileOutput(const int &targetindex
                                   , QString newstring
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;

    m_oldstring = pxlg.fetch(S_OUTPUTFILE, ATTR_NONE, m_cache->at(m_targetindex));

}

void StringFileOutput::undo()
{
    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Search outputfile changed. ") \
            + QString("^(%1)").arg(m_targetindex));
}

void StringFileOutput::redo()
{
    pxlg.replaceElementList(S_OUTPUTFILE, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Search outputfile changed. ") \
            + QString("^(%1)").arg(m_targetindex));
}

int StringFileOutput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(S_OUTPUTFILE);
}

bool StringFileOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringFileOutput *com = static_cast<const StringFileOutput*>(other);
    m_newstring = com->m_newstring;
    return true;
}
