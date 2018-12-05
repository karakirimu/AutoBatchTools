#include "edittextcommand.h"

EditTextCommand::EditTextCommand(const int &targetindex
                                 , QString newstring
                                 , const QString &objname
                                 , QList<QList<QStringList> *> *cache
                                 , QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_targetindex = targetindex;
    m_newstring = newstring;

    m_objname = objname;
    m_cache = cache;

    ProcessXmlListGenerator x;
    x.getListStructure(cache->at(m_targetindex), &posinfo);
    int iddata = id();

    if(iddata == x.getId(S_SEPARATOR)){
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2)
                .at(1);

    }else if(iddata == x.getId(S_OUTPUTFILE)){
        m_oldstring = m_cache->at(m_targetindex)
                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4)
                .at(1);

    }else if(iddata == x.getId(I_NAME)){
        m_oldstring = m_cache->at(m_targetindex)->at(1).at(1);

    }else if(iddata == x.getId(I_VERSION)){
        m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);

    }else if(iddata == x.getId(I_AUTHOR)){
        m_oldstring = m_cache->at(m_targetindex)->at(3).at(1);

    }else if(iddata == x.getId(I_DESCRIPTION)){
        m_oldstring = m_cache->at(m_targetindex)->at(4).at(1);
    }

//    switch (id()) {
//    case ProcessXmlListGenerator::SEPARATOR:
//        m_oldstring = m_cache->at(m_targetindex)
//                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2)
//                .at(1);
//        break;
//    case ProcessXmlListGenerator::OUTPUTFILE:
//        m_oldstring = m_cache->at(m_targetindex)
//                ->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4)
//                .at(1);
//        break;
//    case ProcessXmlListGenerator::INFO_NAME:
//        m_oldstring = m_cache->at(m_targetindex)->at(1).at(1);
//        break;
//    case ProcessXmlListGenerator::INFO_VER:
//         m_oldstring = m_cache->at(m_targetindex)->at(2).at(1);
//        break;
//    case ProcessXmlListGenerator::INFO_AUTHOR:
//         m_oldstring = m_cache->at(m_targetindex)->at(3).at(1);
//        break;
//    case ProcessXmlListGenerator::INFO_DESCRIPT:
//         m_oldstring = m_cache->at(m_targetindex)->at(4).at(1);
//        break;
//    default:
//        break;
//    }
}

void EditTextCommand::undo()
{
    if(m_cache->isEmpty()) return;

    QStringList alist;
    int iddata = id();

    ProcessXmlListGenerator pxg;
    if(iddata == pxg.getId(S_SEPARATOR)){
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));

    }else if(iddata == pxg.getId(S_OUTPUTFILE)){
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);
        setText(QObject::tr("Change text at Search Output file"));

    }else if(iddata == pxg.getId(I_NAME)){
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));

    }else if(iddata == pxg.getId(I_VERSION)){
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));

    }else if(iddata == pxg.getId(I_AUTHOR)){
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));

    }else if(iddata == pxg.getId(I_DESCRIPTION)){
        alist = m_cache->at(m_targetindex)->at(4);
        alist.replace(1, m_oldstring);
        m_cache->at(m_targetindex)->replace(4, alist);
        setText(QObject::tr("Change description"));

    }

//    switch (id()) {
//    case ProcessXmlListGenerator::SEPARATOR:
//        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
//        setText(QObject::tr("Change text at Search Separator"));
//        break;
//    case ProcessXmlListGenerator::OUTPUTFILE:
//        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);
//        setText(QObject::tr("Change text at Search Output file"));
//        break;
//    case ProcessXmlListGenerator::INFO_NAME:
//        alist = m_cache->at(m_targetindex)->at(1);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(1, alist);
//        setText(QObject::tr("Change name"));
//        break;
//    case ProcessXmlListGenerator::INFO_VER:
//        alist = m_cache->at(m_targetindex)->at(2);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(2, alist);
//        setText(QObject::tr("Change version"));
//        break;
//    case ProcessXmlListGenerator::INFO_AUTHOR:
//        alist = m_cache->at(m_targetindex)->at(3);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(3, alist);
//        setText(QObject::tr("Change author"));
//        break;
//    case ProcessXmlListGenerator::INFO_DESCRIPT:
//        alist = m_cache->at(m_targetindex)->at(4);
//        alist.replace(1, m_oldstring);
//        m_cache->at(m_targetindex)->replace(4, alist);
//        setText(QObject::tr("Change description"));
//        break;
//    default:
//        break;
//    }
}

void EditTextCommand::redo()
{
    QStringList alist;
    int iddata = id();

//    ProcessXmlListGenerator pxg;
//    if(iddata == pxg.getId(S_SEPARATOR)){

//    }else if(iddata == pxg.getId(S_OUTPUTFILE)){

//    }else if(iddata == pxg.getId(I_NAME)){

//    }else if(iddata == pxg.getId(I_VERSION)){

//    }else if(iddata == pxg.getId(I_AUTHOR)){

//    }else if(iddata == pxg.getId(I_DESCRIPTION)){

//    }

    ProcessXmlListGenerator pxg;
    if(iddata == pxg.getId(S_SEPARATOR)){
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
        setText(QObject::tr("Change text at Search Separator"));

    }else if(iddata == pxg.getId(S_OUTPUTFILE)){
        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);
        setText(QObject::tr("Change text at Search Output file"));

    }else if(iddata == pxg.getId(I_NAME)){
        alist = m_cache->at(m_targetindex)->at(1);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(1, alist);
        setText(QObject::tr("Change name"));

    }else if(iddata == pxg.getId(I_VERSION)){
        alist = m_cache->at(m_targetindex)->at(2);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(2, alist);
        setText(QObject::tr("Change version"));

    }else if(iddata == pxg.getId(I_AUTHOR)){
        alist = m_cache->at(m_targetindex)->at(3);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(3, alist);
        setText(QObject::tr("Change author"));

    }else if(iddata == pxg.getId(I_DESCRIPTION)){
        alist = m_cache->at(m_targetindex)->at(4);
        alist.replace(1, m_newstring);
        m_cache->at(m_targetindex)->replace(4, alist);
        setText(QObject::tr("Change description"));

    }

//    switch (id()) {
//    case ProcessXmlListGenerator::SEPARATOR:
//        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 2, alist);
//        setText(QObject::tr("Change text at Search Separator"));
//        break;
//    case ProcessXmlListGenerator::OUTPUTFILE:
//        alist = m_cache->at(m_targetindex)->at(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(posinfo.value(ProcessXmlListGenerator::SEARCH) + 4, alist);
//        setText(QObject::tr("Change text at Search Output file"));
//        break;
//    case ProcessXmlListGenerator::INFO_NAME:
//        alist = m_cache->at(m_targetindex)->at(1);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(1, alist);
//        setText(QObject::tr("Change name"));
//        break;
//    case ProcessXmlListGenerator::INFO_VER:
//        alist = m_cache->at(m_targetindex)->at(2);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(2, alist);
//        setText(QObject::tr("Change version"));
//        break;
//    case ProcessXmlListGenerator::INFO_AUTHOR:
//        alist = m_cache->at(m_targetindex)->at(3);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(3, alist);
//        setText(QObject::tr("Change author"));
//        break;
//    case ProcessXmlListGenerator::INFO_DESCRIPT:
//        alist = m_cache->at(m_targetindex)->at(4);
//        alist.replace(1, m_newstring);
//        m_cache->at(m_targetindex)->replace(4, alist);
//        setText(QObject::tr("Change description"));
//        break;
//    default:
//        break;
//    }
}

//DEPENDS_XML DEPENDS_UI
int EditTextCommand::id() const
{
    ProcessXmlListGenerator pxg;
    if(m_objname == "separatorLineEdit"){
        return pxg.getId(S_SEPARATOR);
    }else if(m_objname == "outputLineEdit"){
        return pxg.getId(S_OUTPUTFILE);
    }else if(m_objname == "nameLineEdit"){
        return pxg.getId(I_NAME);
    }else if(m_objname == "verLineEdit"){
        return pxg.getId(I_VERSION);
    }else if(m_objname == "authorLineEdit"){
        return pxg.getId(I_AUTHOR);
    }else if(m_objname == "descTextEdit"){
        return pxg.getId(I_DESCRIPTION);
    }else {
        return -1;
    }
}

bool EditTextCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) return false;
    const EditTextCommand *com = static_cast<const EditTextCommand*>(other);
    m_newstring = com->m_newstring;
    return true;
}
