#include "editsearchoutput.h"

EditSearchOutput::EditSearchOutput(const int &targetindex
                                   , int newvalue
                                   , QList<QList<QStringList> *> *cache
                                   , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newvalue = newvalue;
    m_cache = cache;

    if(m_targetindex > 1){
        //init generator
        ProcessXmlListGenerator x;
        x.getListStructure(m_cache->at(m_targetindex), &posinfo);
    }

    m_oldvalue = ((QString)m_cache->at(m_targetindex)
            ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4)
            .at(3)).toInt();
}

void EditSearchOutput::undo()
{
    if(m_cache->isEmpty()) return;

    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
    alist.replace(3, QString::number(m_oldvalue));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);

    setText(QObject::tr("Output Select"));
}

void EditSearchOutput::redo()
{
    QStringList alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
    alist.replace(3, QString::number(m_newvalue));
    m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);

    setText(QObject::tr("Output Select"));
}

int EditSearchOutput::id() const
{
    return ProcessXmlListGenerator::OUTPUT_RADIO;
}

bool EditSearchOutput::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditSearchOutput *com = static_cast<const EditSearchOutput *>(other);
    m_newvalue = com->m_newvalue;
    return true;
}
