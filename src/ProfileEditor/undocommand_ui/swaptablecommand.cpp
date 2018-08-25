#include "swaptablecommand.h"

SwapTableCommand::SwapTableCommand(const int &targetindex
                                   , const int &tablebefore
                                   , const int &tableafter
//                                   , QString newstr
                                   , const QString objname
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_indexbefore = tablebefore;
    m_indexafter = tableafter;
//    m_newstr = newstr;
//    m_oldstr = "";
    m_objname = objname;
    m_cache = cache;

    //init generator
    ProcessXmlListGenerator x;
    QHash<int, int> posinfo;
    x.getListStructure(m_cache->at(m_targetindex), &posinfo);

    switch (id()) {
    case ProcessXmlListGenerator::NCMDCOUNT:
        //define SKIP
        SKIP = posinfo.value(ProcessXmlListGenerator::NORMAL) + 3;
        break;
    case ProcessXmlListGenerator::ECMDCOUNT:
        //define SKIP
        SKIP = posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + 4;
        break;
    default:
        break;
    }

//    m_oldstr = ((QString)m_cache->at(m_targetindex)->at(m_indexbefore + SKIP).at(1));
}

void SwapTableCommand::undo()
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
        setText(QObject::tr("Up table value '%1'").arg(temp.at(1)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down table value '%1'").arg(temp.at(1)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap table value '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter));
        break;
    }
}

void SwapTableCommand::redo()
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
        setText(QObject::tr("Up table value '%1'").arg(temp.at(1)));
        break;
    case -1:
        //DOWN
        setText(QObject::tr("Down table value '%1'").arg(temp.at(1)));
        break;
    default:
        //SWAP
        setText(QObject::tr("Swap table value '%1' at %2 to %3").arg(temp.at(1)).arg(m_indexbefore).arg(m_indexafter));
        break;
    }
}

int SwapTableCommand::id() const
{
    if(m_objname == "cmdTableWidget"){
        return ProcessXmlListGenerator::NCMDCOUNT;
    }else if(m_objname == "extrafuncTableWidget"){
        return ProcessXmlListGenerator::ECMDCOUNT;
    }else{
        return -1;
    }
}
