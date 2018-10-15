#ifndef SWAPEXECTABLE_H
#define SWAPEXECTABLE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class SwapExecTable : public QUndoCommand
{
public:
    SwapExecTable(const int &targetindex
                  , const int &tablebefore
                  , const int &tableafter
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

#endif // SWAPEXECTABLE_H
