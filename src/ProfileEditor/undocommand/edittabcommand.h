#ifndef EDITTABCOMMAND_H
#define EDITTABCOMMAND_H

#include <QUndoCommand>
#include <xmllistgenerator.h>

class EditTabCommand : public QUndoCommand
{
public:
    EditTabCommand(const int &targetindex
                   , const int &newid
                   , QList<QList<QStringList> *> *cache
                   , QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    void selectIndex(int index);

    int m_targetindex;
    int m_oldid;
    QString m_oldauto;
    int m_newid;
    QString m_newauto;
    QList<QList<QStringList> *> *m_cache;
};

#endif // EDITTABCOMMAND_H
