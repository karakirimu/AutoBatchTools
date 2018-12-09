#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(const int &targetindex
                  , QList<QStringList> *deleted
                  , QList<QList<QStringList> *> *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int m_targetindex;
//    QList<QStringList> *m_deleted;
    QList<QStringList> m_delcopy;
    QList<QList<QStringList> *> *m_cache;
};

#endif // DELETECOMMAND_H
