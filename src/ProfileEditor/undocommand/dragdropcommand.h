#ifndef DRAGDROPCOMMAND_H
#define DRAGDROPCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class DragDropCommand : public QUndoCommand
{
public:
    DragDropCommand(const QList<int> &beforeindex
                    , const int &afterindex
                    , QList<QList<QStringList> *> *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    QList<int> m_beforeindex;
    int m_afterindex;

    QList<QList<QStringList> *> *m_cache;

};

#endif // DRAGDROPCOMMAND_H
