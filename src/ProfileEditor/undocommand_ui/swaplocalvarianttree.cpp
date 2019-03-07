#include "swaplocalvarianttree.h"

SwapLocalVariantTree::SwapLocalVariantTree(const int &localindex
                                         , const int &before
                                         , const int &after
                                         , QList<QList<QStringList> *> *cache
                                         , QUndoCommand *parent)
    :QUndoCommand (parent)
{
    m_targetindex = localindex;
    m_indexbefore = before;
    m_indexafter = after;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(m_targetindex));
}

void SwapLocalVariantTree::undo()
{
    QStringList temp;
    temp = m_cache->at(m_targetindex)->at(m_indexbefore + SKIP);

    //swap
    m_cache->at(m_targetindex)->replace(m_indexbefore + SKIP
                                        , m_cache->at(m_targetindex)->at(m_indexafter + SKIP));
    m_cache->at(m_targetindex)->replace(m_indexafter + SKIP, temp);

    int condition = m_indexafter - m_indexbefore;
    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up local at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_LV_SWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down local at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_LV_SWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap local at %1 %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_LV_SWAP));
        break;
    }
}

void SwapLocalVariantTree::redo()
{
    QStringList temp;
    temp = m_cache->at(m_targetindex)->at(m_indexafter + SKIP);

    //swap
    m_cache->at(m_targetindex)->replace(m_indexafter + SKIP
                                        , m_cache->at(m_targetindex)->at(m_indexbefore + SKIP));
    m_cache->at(m_targetindex)->replace(m_indexbefore + SKIP, temp);

    int condition = m_indexafter - m_indexbefore;

    switch (condition) {
    case 1:
        //UP
        setText(QObject::tr("Up local at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_LV_SWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down local at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_LV_SWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap local at %1 %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_LV_SWAP));
        break;
    }
}

int SwapLocalVariantTree::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(L_SWAP_TABLE);
}
