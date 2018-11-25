#ifndef EDITVALUECOMMAND_H
#define EDITVALUECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditValueCommand : public QUndoCommand
{
public:
    EditValueCommand(const int &targetindex
//                     , const int oldvalue
                     , int newvalue
                     , const QString &objname
                     , QList<QList<QStringList> *> *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    int m_newvalue;

private:
    int m_targetindex;
    int m_oldvalue;
    QString m_objname;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;

};

#endif // EDITVALUECOMMAND_H
