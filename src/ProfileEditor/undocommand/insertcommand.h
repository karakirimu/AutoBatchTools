#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class InsertCommand : public QUndoCommand
{
public:
    InsertCommand(const int &targetindex
                  , QList<QStringList> *inserted
                  , QList<QList<QStringList> *> *cache
                  , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int m_targetindex;
//    QList<QStringList> *m_inserted;
    QList<QStringList> m_inscopy;
    QList<QList<QStringList> *> *m_cache;
};

#endif // INSERTCOMMAND_H
