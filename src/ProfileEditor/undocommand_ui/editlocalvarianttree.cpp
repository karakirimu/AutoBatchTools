#include "editlocalvarianttree.h"

EditLocalVariantTree::EditLocalVariantTree(const int &targetindex
                                           , const int &treeindex
                                           , QStringList variants
                                           , const int operation
                                           , QList<QList<QStringList> *> *cache
                                           , QUndoCommand *parent)
    :QUndoCommand (parent)
{
    m_targetindex = targetindex;
    m_treeindex = treeindex;

    m_newvar = variants;
    m_oldvar.clear();

    m_operation = operation;
    m_cache = cache;

    SKIP = pxlg.fetchCmdFirstPos(L_VAR_COUNT, m_cache->at(m_targetindex));

    if(m_operation == ProcessXmlListGenerator::TREE_EDIT
            || m_operation == ProcessXmlListGenerator::TREE_DELETE){
        m_oldvar.append(m_cache->at(m_targetindex)->at(m_treeindex + SKIP).at(1));
        m_oldvar.append(m_cache->at(m_targetindex)->at(m_treeindex + SKIP).at(3));
    }
}

void EditLocalVariantTree::undo()
{
    qDebug() << "[EditLocalVariantTree::undo()] treeindex : " << m_treeindex;

    QStringList alist;
    switch (m_operation) {
    case ProcessXmlListGenerator::TREE_ADD:
        //delete
        m_cache->at(m_targetindex)->removeAt(m_treeindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Add local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_LV_DEL));

        break;

    case ProcessXmlListGenerator::TREE_EDIT:
        pxlg.replaceElementList(m_treeindex, m_targetindex, m_oldvar, SKIP, m_cache);

        setText(QObject::tr("Edit local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_treeindex).arg(UNDOREDO_LV_EDIT));
        break;

    case ProcessXmlListGenerator::TREE_INSERT:
        m_cache->at(m_targetindex)->removeAt(m_treeindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_treeindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_LV_DEL));
        break;

    case ProcessXmlListGenerator::TREE_DELETE:
        alist = ProcessXmlListGenerator::createVariantElement(m_oldvar);
        m_cache->at(m_targetindex)->insert(m_treeindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Delete local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_treeindex).arg(UNDOREDO_LV_INS));
        break;

    default:
        break;
    }
}

void EditLocalVariantTree::redo()
{
    QStringList alist;
    qDebug() << "[EditLocalVariantTree::redo()] treeindex : " << m_treeindex;


    switch (m_operation) {
    case ProcessXmlListGenerator::TREE_ADD:
        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
        m_cache->at(m_targetindex)->insert(m_treeindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Add local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_LV_ADD));

        break;
    case ProcessXmlListGenerator::TREE_EDIT:
        pxlg.replaceElementList(m_treeindex, m_targetindex, m_newvar, SKIP, m_cache);

        setText(QObject::tr("Edit local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_treeindex).arg(UNDOREDO_LV_EDIT));
        break;
    case ProcessXmlListGenerator::TREE_INSERT:

        alist = ProcessXmlListGenerator::createVariantElement(m_newvar);
        m_cache->at(m_targetindex)->insert(m_treeindex + SKIP, alist);

        updateCounter(true);

        setText(QObject::tr("Insert local at %1 \'%2\'").arg(m_treeindex).arg(m_newvar.at(0)) \
                + QString(" ^(%1,%2,%3)").arg(m_targetindex).arg(m_treeindex).arg(UNDOREDO_LV_INS));
        break;
    case ProcessXmlListGenerator::TREE_DELETE:
        //add
        m_cache->at(m_targetindex)->removeAt(m_treeindex + SKIP);

        updateCounter(false);

        setText(QObject::tr("Delete local at %1").arg(m_treeindex) \
                + QString(" ^(%1,%2)").arg(m_treeindex).arg(UNDOREDO_LV_DEL));
        break;
    default:
        break;
    }
}

int EditLocalVariantTree::id() const
{
    ProcessXmlListGenerator pxg;

    switch (m_operation) {
    case ProcessXmlListGenerator::TREE_ADD:
        return pxg.getId(UNDOREDO_LV_ADD);

    case ProcessXmlListGenerator::TREE_EDIT:
        return pxg.getId(UNDOREDO_LV_EDIT);

    case ProcessXmlListGenerator::TREE_INSERT:
        return pxg.getId(UNDOREDO_LV_INS);

    case ProcessXmlListGenerator::TREE_DELETE:
        return pxg.getId(UNDOREDO_LV_DEL);

    }

    return pxg.getId(L_VAR_COUNT);
}

bool EditLocalVariantTree::mergeWith(const QUndoCommand *other)
{
    Q_UNUSED(other)
    return false;
}

void EditLocalVariantTree::updateCounter(bool ascend)
{
    QStringList alist;
    //index size update
    alist = m_cache->at(m_targetindex)->at(SKIP - 1);
    int rcount = static_cast<QString>(pxlg.fetch(L_VAR_COUNT, ATTR_NONE, m_cache->at(m_targetindex))).toInt();
    if(ascend){
        alist.replace(1, QString::number(rcount + 1));
    }else{
        alist.replace(1, QString::number(rcount - 1));
    }
    m_cache->at(m_targetindex)->replace(SKIP - 1, alist);
}
