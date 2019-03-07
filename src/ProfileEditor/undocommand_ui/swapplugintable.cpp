#include "swapplugintable.h"

SwapPluginTable::SwapPluginTable(const int &targetindex
                                 , const int &tablebefore
                                 , const int &tableafter
                                 , QList<QList<QStringList> *> *cache
                                 , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_indexbefore = tablebefore;
    m_indexafter = tableafter;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(PL_CMD, m_cache->at(m_targetindex));

}

void SwapPluginTable::undo()
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
        setText(QObject::tr("Up plugin at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_PL_TABLESWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down plugin at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_PL_TABLESWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap plugin at %1 %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexafter).arg(m_indexbefore).arg(UNDOREDO_PL_TABLESWAP));
        break;
    }
}

void SwapPluginTable::redo()
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
        setText(QObject::tr("Up plugin at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_PL_TABLESWAP));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down plugin at %1").arg(temp.at(1)) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_PL_TABLESWAP));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap plugin at %1 %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter) \
                + QString(" ^(%1,%2,%3)").arg(m_indexbefore).arg(m_indexafter).arg(UNDOREDO_PL_TABLESWAP));
        break;
    }
}

int SwapPluginTable::id() const
{
    ProcessXmlListGenerator pxg;
    return pxg.getId(PL_CMDARGCOUNT);
}
