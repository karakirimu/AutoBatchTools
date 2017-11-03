#ifndef EDITTABLECOMMAND_H
#define EDITTABLECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditTableCommand : public QUndoCommand
{
public:
    EditTableCommand(const int &targetindex
                     ,const int &tableindex
                     ,QString newstr
                     ,const int operation
                     ,const QString objname
                     ,QList<QList<QStringList> *> *cache
                     ,QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    QString m_newstr;

    int operation() const;

private:
    void updateIndex(int count);
    void updateCounter(bool ascend);

    int m_targetindex;
    int m_tableindex;
    QString m_objname;
    QString m_oldstr;
    int m_operation;
    QList<QList<QStringList> *> *m_cache;

    int SKIP;

};

#endif // EDITTABLECOMMAND_H
