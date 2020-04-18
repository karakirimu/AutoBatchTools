#include "checkeditsearchinput.h"

CheckEditSearchInput::CheckEditSearchInput(const int &targetindex
                                           , const bool &newcheck
                                           , QList<QList<QStringList> *> *cache
                                           , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditSearchInput::undo()
{
    pxlg.replaceElementList(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Search input %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

void CheckEditSearchInput::redo()
{
    pxlg.replaceElementList(I_FILEINPUT_SEARCHCHECK, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Search input %1").arg(m_newcheck) \
            + QString(" ^(%1,%2)").arg(m_targetindex).arg(UNDOREDO_EDIT));
}

int CheckEditSearchInput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_FILEINPUT_SEARCHCHECK);
}
