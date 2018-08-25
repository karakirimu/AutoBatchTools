#ifndef EXTRATABLECOMMAND_H
#define EXTRATABLECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class ExtraTableCommand : public QUndoCommand
{
public:
    ExtraTableCommand(const int &targetindex
                      ,const int &tableindex
                      ,QString newstr
                      ,const int operation
                      ,QList<QList<QStringList> *> *cache
                      ,QUndoCommand *parent = nullptr);

    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

    int id() const Q_DECL_OVERRIDE;
//    bool mergeWith(const QUndoCommand *other) Q_DECL_OVERRIDE;

    QString m_newstr;

private:
    int m_targetindex;
    int m_tableindex;
    QString m_oldstr;
    int m_operation;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;

};

#endif // EXTRATABLECOMMAND_H
