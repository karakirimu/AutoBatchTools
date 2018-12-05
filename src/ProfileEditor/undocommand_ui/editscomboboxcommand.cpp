#include "editscomboboxcommand.h"

EditScomboBoxCommand::EditScomboBoxCommand(const int &targetindex
                                           , const QString &newstring
                                           , const int &newsearchindex
                                           , QList<QList<QStringList> *> *cache
                                           , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    m_newindex = newsearchindex;

    if(m_targetindex > 1){
        // sname
        m_oldstring = pxlg.fetch(S_NAME, ATTR_NONE, m_cache->at(m_targetindex));
        m_oldindex = static_cast<QString>(pxlg.fetch(S_NAME, ATTR_POSNUM, m_cache->at(m_targetindex))).toInt();
    }else {
        //info "fsname" position
        m_oldstring = pxlg.fetch(I_FILESEARCH_NAME, ATTR_NONE, m_cache->at(m_targetindex));
        m_oldindex = static_cast<QString>(pxlg.fetch(I_FILESEARCH_NAME, ATTR_POSNUM, m_cache->at(m_targetindex))).toInt();
    }
}

void EditScomboBoxCommand::undo()
{
    if(m_targetindex > 1){
        pxlg.replaceElementList(S_NAME, ATTR_NONE, m_targetindex, m_oldstring, m_cache);
        pxlg.replaceElementList(S_NAME, ATTR_POSNUM, m_targetindex, QString::number(m_oldindex), m_cache);

        setText(QObject::tr("Search profile to") + m_oldstring \
                + QString(" ^(%1)").arg(m_targetindex));
    }else{
        pxlg.replaceElementList(I_FILESEARCH_NAME, ATTR_NONE, m_targetindex, m_oldstring, m_cache);
        pxlg.replaceElementList(I_FILESEARCH_NAME, ATTR_POSNUM, m_targetindex, QString::number(m_oldindex), m_cache);

        setText(QObject::tr("Input search profile to") + m_oldstring \
                + QString(" ^(%1)").arg(m_targetindex));
    }
}

void EditScomboBoxCommand::redo()
{
    if(m_targetindex > 1){
        pxlg.replaceElementList(S_NAME, ATTR_NONE, m_targetindex, m_newstring, m_cache);
        pxlg.replaceElementList(S_NAME, ATTR_POSNUM, m_targetindex, QString::number(m_newindex), m_cache);

        setText(QObject::tr("Search profile to") + m_newstring \
                + QString(" ^(%1)").arg(m_targetindex));
    }else{
        pxlg.replaceElementList(I_FILESEARCH_NAME, ATTR_NONE, m_targetindex, m_newstring, m_cache);
        pxlg.replaceElementList(I_FILESEARCH_NAME, ATTR_POSNUM, m_targetindex, QString::number(m_newindex), m_cache);

        setText(QObject::tr("Input search profile to") + m_newstring \
                + QString(" ^(%1)").arg(m_targetindex));
    }
}

int EditScomboBoxCommand::id() const
{
    ProcessXmlListGenerator pxg;
    if(m_targetindex > 1){
        // sname
        return pxg.getId(S_NAME);
    }else {
        //info "fsname" position
        return pxg.getId(I_FILESEARCH_NAME);
    }
}
