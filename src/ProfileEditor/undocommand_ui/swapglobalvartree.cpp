#include "swapglobalvartree.h"

SwapGlobalVarTree::SwapGlobalVarTree(const int &before
                                     , const int &after
                                     , QUndoCommand *parent)
    :QUndoCommand (parent)
{
    m_indexbefore = before;
    m_indexafter = after;
}

void SwapGlobalVarTree::undo()
{
    sxml.swapItem(m_indexafter, m_indexbefore);

    QList<QStringList> newvar;
    QString vari = "";
    if(sxml.readItem(m_indexbefore, &newvar)){
        vari = newvar.at(0).at(1);
    }

    int condition = m_indexbefore - m_indexafter;
    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up global at %1").arg(vari) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down global at %1").arg(vari) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap global at %1 %2 to %3").arg(vari).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    }
}

void SwapGlobalVarTree::redo()
{
    sxml.swapItem(m_indexbefore, m_indexafter);

    QList<QStringList> newvar;
    QString vari = "";
    if(sxml.readItem(m_indexafter, &newvar)){
        vari = newvar.at(0).at(1);
    }

    int condition = m_indexbefore - m_indexafter;

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up global at %1").arg(vari) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down global at %1").arg(vari) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap global at %1 %2 to %3").arg(vari).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UiCommandMap::Id(UiCommandMap::GV_SWAP_TREE)));
        break;
    }
}

int SwapGlobalVarTree::id() const
{
    //todo:
//    ProcessXmlListGenerator pxg;
//    return pxg.getId(L_SWAP_TABLE);
    return UiCommandMap::GV_SWAP_TREE;
}
