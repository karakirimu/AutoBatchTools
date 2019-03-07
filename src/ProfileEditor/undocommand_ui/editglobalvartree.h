#ifndef EDITGLOBALVARTREE_H
#define EDITGLOBALVARTREE_H

#include <QUndoCommand>
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @brief The EditGlobalVarTree class
 * newstrlist structure
 * 0       :1       :
 * variant :value   :
 */

class EditGlobalVarTree : public QUndoCommand
{
public:
    EditGlobalVarTree(const int &treeindex
                      , QStringList variants
                      , const int &operation
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QStringList m_newvar;

private:
//    void updateCounter(bool ascend);
//    void setStrBuilderFormat(QList<QStringList> *internal, QStringList *var);

    int m_treeindex;
    int m_tableindex;
    QStringList m_oldvar;
    int m_operation;
//    QList<QList<QStringList> *> *m_cache;

    StringXmlBuilder sxml;

//    int SKIP;
};

#endif // EDITGLOBALVARTREE_H
