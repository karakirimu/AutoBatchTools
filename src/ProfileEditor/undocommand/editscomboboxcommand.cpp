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
        ProcessXmlListGenerator x;
        x.getListStructure(cache->at(m_targetindex), &posinfo);
        m_indexpos = posinfo.value(ProcessXmlListGenerator::SEARCH) + 1;
    }else {
        //info "fsname" position
        m_indexpos = 7;
    }

    m_oldstring = m_cache->at(m_targetindex)
                ->at(m_indexpos)
                .at(1);
    m_oldindex = ((QString)(m_cache->at(m_targetindex)
                ->at(m_indexpos)
                .at(3))).toInt();
}

void EditScomboBoxCommand::undo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(m_indexpos);
    list.replace(1, m_oldstring);
    list.replace(3, QString::number(m_oldindex));
    m_cache->at(m_targetindex)->replace(m_indexpos, list);

    if(m_targetindex > 1){
        setText(QObject::tr("Search profile to ") + m_oldstring);
    }else{
        setText(QObject::tr("Input search profile to ") + m_oldstring);
    }
}

void EditScomboBoxCommand::redo()
{
    QStringList list;
    list = m_cache->at(m_targetindex)->at(m_indexpos);
    list.replace(1, m_newstring);
    list.replace(3, QString::number(m_newindex));
    m_cache->at(m_targetindex)->replace(m_indexpos, list);

    if(m_targetindex > 1){
        setText(QObject::tr("Search profile to ") + m_newstring);
    }else{
        setText(QObject::tr("Input search profile to ") + m_newstring);
    }
}
