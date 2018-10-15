#ifndef EDITFULLCOMMAND_H
#define EDITFULLCOMMAND_H

#include <QHash>
#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditLocalVarTable : public QUndoCommand
{
public:
    EditLocalVarTable(const int &targetindex
                    , QList<QStringList> *changed
                    , QList<QList<QStringList> *> *cache
                    , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QList<QStringList> *m_changed;

private:
    int m_targetindex;
    QList<QStringList> *m_before;
    QList<QList<QStringList> *> *m_cache;
    static const int MAGIC = 1;

};

#endif // EDITFULLCOMMAND_H
