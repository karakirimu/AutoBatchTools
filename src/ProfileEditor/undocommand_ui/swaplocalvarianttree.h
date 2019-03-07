#ifndef SWAPVARIANTTREE_H
#define SWAPVARIANTTREE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class SwapLocalVariantTree : public QUndoCommand
{
public:
    SwapLocalVariantTree(const int &localindex
                         , const int &before
                         , const int &after
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
    int id() const override;

private:
    int m_targetindex;
    int m_indexbefore;
    int m_indexafter;
    QString m_objname;

    QList<QList<QStringList> *> *m_cache;

    ProcessXmlListGenerator pxlg;

    int SKIP;
};

#endif // SWAPVARIANTTREE_H
