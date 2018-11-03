#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class AddCommand : public QUndoCommand
{
public:
    AddCommand(const int &targetindex
               , QList<QStringList> *added
               , QList<QList<QStringList> *> *cache
               , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
    int m_targetindex;
    QList<QStringList> *m_added;
    QList<QStringList> m_addcopy;
    QList<QList<QStringList> *> *m_cache;
};

#endif // ADDCOMMAND_H
