#ifndef EDITVARIANTTREE_H
#define EDITVARIANTTREE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

/**
 * @brief The EditLocalVariantTree class
 * newstrlist structure
 * 0       :1       :
 * variant :value   :
 */

class EditLocalVariantTree : public QUndoCommand
{
public:
    EditLocalVariantTree(const int &targetindex
                         , const int &treeindex
                         , QStringList variants
                         , const int operation
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QStringList m_newvar;

private:
    void updateIndex(int count);
    void updateCounter(bool ascend);

    int m_targetindex;
    int m_treeindex;
    QStringList m_oldvar;
    int m_operation;

    QList<QList<QStringList> *> *m_cache;
    ProcessXmlListGenerator pxlg;

    int SKIP;
};

#endif // EDITVARIANTTREE_H
