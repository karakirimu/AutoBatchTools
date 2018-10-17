#include "stringprojectversion.h"

StringProjectVersion::StringProjectVersion(const int &targetindex
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

    //info version
//    m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);
    m_oldstring = pxlg.fetch(I_VERSION, ATTR_NONE, m_cache->at(m_targetindex));

}

void StringProjectVersion::undo()
{
//    if(m_cache->isEmpty()) return;

//    QStringList alist = m_cache->at(m_targetindex)->at(2);
//    alist.replace(1, m_oldstring);
//    m_cache->at(m_targetindex)->replace(2, alist);
    pxlg.replaceElementList(I_VERSION, ATTR_NONE, m_targetindex, m_oldstring, m_cache);

    setText(QObject::tr("Version changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

void StringProjectVersion::redo()
{
//    QStringList alist = m_cache->at(m_targetindex)->at(2);
//    alist.replace(1, m_newstring);
//    m_cache->at(m_targetindex)->replace(2, alist);
    pxlg.replaceElementList(I_VERSION, ATTR_NONE, m_targetindex, m_newstring, m_cache);

    setText(QObject::tr("Version changed ") \
            + QString("^(%1)").arg(m_targetindex));
}

int StringProjectVersion::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_VERSION);
}

bool StringProjectVersion::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const StringProjectVersion *com = static_cast<const StringProjectVersion*>(other);
    m_newstring = com->m_newstring;
    return true;
}
