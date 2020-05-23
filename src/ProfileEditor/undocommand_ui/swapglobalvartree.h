#ifndef SWAPGLOBALVARTREE_H
#define SWAPGLOBALVARTREE_H

#include <QUndoCommand>
#include <../stringxmlbuilder/stringxmlbuilder.h>
//#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <uicommandmap.h>

class SwapGlobalVarTree : public QUndoCommand
{
public:
    SwapGlobalVarTree(const int &before
                      , const int &after
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

private:
    int m_indexbefore;
    int m_indexafter;
    QString m_objname;

    StringXmlBuilder sxml;

};

#endif // SWAPGLOBALVARTREE_H
