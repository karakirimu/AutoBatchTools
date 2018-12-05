#include "checkeditallowinput.h"

CheckEditAllowInput::CheckEditAllowInput(const int &targetindex
                               , const bool &newcheck
                               , QList<QList<QStringList> *> *cache
                               , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newcheck = VariantConverter::boolToString(newcheck);
    m_cache = cache;

    m_oldcheck = pxlg.fetch(I_FILEINPUT, ATTR_NONE, m_cache->at(m_targetindex));
}

void CheckEditAllowInput::undo()
{
    pxlg.replaceElementList(I_FILEINPUT, ATTR_NONE, m_targetindex, m_oldcheck, m_cache);

    setText(QObject::tr("Allow input %1").arg(m_newcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

void CheckEditAllowInput::redo()
{
    pxlg.replaceElementList(I_FILEINPUT, ATTR_NONE, m_targetindex, m_newcheck, m_cache);

    setText(QObject::tr("Allow input %1").arg(m_newcheck) \
            + QString(" ^(%1)").arg(m_targetindex));
}

int CheckEditAllowInput::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(I_FILEINPUT);
}
