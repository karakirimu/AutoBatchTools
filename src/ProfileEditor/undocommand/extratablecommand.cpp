#include "extratablecommand.h"

ExtraTableCommand::ExtraTableCommand(const int &targetindex
                                     , const int &tableindex
                                     , QString newstr
                                     , const int operation
                                     , QList<QList<QStringList> *> *cache
                                     , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_tableindex = tableindex;
    m_newstr = newstr;
    m_operation = operation;
    m_cache = cache;

    //init generator
    ProcessXmlListGenerator x;
    x.getListStructure(m_cache->at(m_targetindex), &posinfo);

    //get old value
//    int rcount = 0;

    if(m_operation != ProcessXmlListGenerator::TABLE_ADD
            || m_operation != ProcessXmlListGenerator::TABLE_INSERT){
    //        rcount = ((QString)m_cache->at(m_targetindex)
    //                      ->at(posinfo.value(XmlListGenerator::EXTRAFUNC) + 3)).toInt();
        m_oldstr = ((QString)m_cache->at(m_targetindex)
                    ->at(posinfo.value(ProcessXmlListGenerator::EXTRAFUNC) + m_tableindex + 4).at(1));
    }
}

void ExtraTableCommand::undo()
{
    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(posinfo.value(ProcessXmlListGenerator::NORMAL)+ m_tableindex + 4);

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL)+ m_tableindex + 4);
        alist.replace(1, m_oldstr);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL)+ m_tableindex + 4, alist);

        setText(QObject::tr("Set table value \'%1\' at %2").arg(m_oldstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:

        break;
    case ProcessXmlListGenerator::TABLE_DELETE:

        break;
    default:
        break;
    }
}

void ExtraTableCommand::redo()
{
    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TABLE_ADD:

        break;
    case ProcessXmlListGenerator::TABLE_EDIT:
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::NORMAL)+ m_tableindex + 4);
        alist.replace(1, m_newstr);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::NORMAL)+ m_tableindex + 4, alist);

        setText(QObject::tr("Set table value \'%1\' at %2").arg(m_newstr).arg(QString::number(m_tableindex)));
        break;
    case ProcessXmlListGenerator::TABLE_INSERT:

        break;
    case ProcessXmlListGenerator::TABLE_DELETE:
        //add

        break;
    default:
        break;
    }
}

int ExtraTableCommand::id() const
{
    return ProcessXmlListGenerator::ECMDCOUNT;
}

bool ExtraTableCommand::mergeWith(const QUndoCommand *other)
{

}
