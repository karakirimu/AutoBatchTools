#include "editcomboboxcommand.h"

EditComboBoxCommand::EditComboBoxCommand(const int &targetindex
                                         , const QString newstring
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;
    m_cache = cache;

    m_oldstring = pxlg.fetch(S_VARIANT, ATTR_NONE, m_cache->at(m_targetindex));

}

void EditComboBoxCommand::undo()
{
    pxlg.replaceElementList(S_VARIANT, ATTR_NONE, m_targetindex, m_oldstring, m_cache);
    setText(QObject::tr("Return variant to %1").arg(m_oldstring) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void EditComboBoxCommand::redo()
{
    pxlg.replaceElementList(S_VARIANT, ATTR_NONE, m_targetindex, m_newstring, m_cache);
    setText(QObject::tr("Return variant to %1").arg(m_newstring) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int EditComboBoxCommand::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(S_VARIANT);
}

bool EditComboBoxCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditComboBoxCommand *com = static_cast<const EditComboBoxCommand*>(other);
    m_newstring = com->m_newstring;
    return true;
}
