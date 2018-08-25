#include "swapexectable.h"

SwapExecTable::SwapExecTable(const int &targetindex
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

    //init generator
    ProcessXmlListGenerator x;
    QHash<int, int> posinfo;
    x.getListStructure(m_cache->at(m_targetindex), &posinfo);

    //define SKIP
    SKIP = posinfo.value(ProcessXmlListGenerator::NORMAL) + 3;
}

void SwapExecTable::undo()
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
        setText(QObject::tr("Up exec arg '%1'").arg(temp.at(1)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down exec arg '%1'").arg(temp.at(1)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap exec arg '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter));
        break;
    }
}

void SwapExecTable::redo()
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
        setText(QObject::tr("Up exec arg '%1'").arg(temp.at(1)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down exec arg '%1'").arg(temp.at(1)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap exec arg '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter));
        break;
    }
}

int SwapExecTable::id() const
{
    return ProcessXmlListGenerator::NCMDCOUNT;
}