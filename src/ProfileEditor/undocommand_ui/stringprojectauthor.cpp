#include "stringprojectauthor.h"

StringProjectAuthor::StringProjectAuthor(const int &targetindex
                                         , QString newstring
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;

//    ProcessXmlListGenerator x;
//    x.getListStructure(cache->at(m_targetindex), &posinfo);

//    m_oldstring = m_cache->at(m_targetindex)->at(3).at(1);
    m_oldstring = pxlg.fetch(I_AUTHOR, ATTR_NONE, m_cache->at(m_targetindex));

}

void StringProjectAuthor::undo()
{
//    if(m_cache->isEmpty()) return;

//    QStringList alist = m_cache->at(m_targetindex)->at(3);
//    alist.replace(1, m_oldstring);
//    m_cache->at(m_targetindex)->replace(3, alist);
    pxlg.replaceElementList(I_AUTHOR, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Author changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

void StringProjectAuthor::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(3);
//    alist.replace(1, m_newstring);
//    m_cache->at(m_targetindex)->replace(3, alist);
    pxlg.replaceElementList(I_AUTHOR, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Author changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

int StringProjectAuthor::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_AUTHOR);
}

bool StringProjectAuthor::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectAuthor *com = static_cast<const StringProjectAuthor*>(other);
    m_newstring = com->m_newstring;
    return true;
}
