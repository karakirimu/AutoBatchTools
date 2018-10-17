#include "combopluginselect.h"

ComboPluginSelect::ComboPluginSelect(const int &targetindex
                                     , const QString newstring
                                     , const QString newfile
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_cache = cache;
    m_newfile = newfile;

    m_oldstring = pxlg.fetch(PL_NAME, ATTR_NONE, m_cache->at(m_targetindex));
    m_oldfile = pxlg.fetch(PL_FILEPATH, ATTR_NONE, m_cache->at(m_targetindex));
}

void ComboPluginSelect::undo()
{
    pxlg.replaceElementList(PL_NAME, ATTR_NONE, m_targetindex, m_oldstring, m_cache);
    pxlg.replaceElementList(PL_FILEPATH, ATTR_NONE, m_targetindex, m_oldfile, m_cache);

    setText(QObject::tr("Plugins to %1 ").arg(m_oldstring) \
            + QString("^(%1)").arg(m_targetindex));
}

void ComboPluginSelect::redo()
{
    pxlg.replaceElementList(PL_NAME, ATTR_NONE, m_targetindex, m_newstring, m_cache);
    pxlg.replaceElementList(PL_FILEPATH, ATTR_NONE, m_targetindex, m_newfile, m_cache);

    setText(QObject::tr("Plugins to %1 ").arg(m_newstring) \
            + QString("^(%1)").arg(m_targetindex));
}

int ComboPluginSelect::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(PL_NAME);
}

bool ComboPluginSelect::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const ComboPluginSelect *com = static_cast<const ComboPluginSelect*>(other);
    m_newstring = com->m_newstring;
    m_newfile = com->m_newfile;
    return true;
}
