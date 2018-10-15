#ifndef SWAPTABLECOMMAND_H
#define SWAPTABLECOMMAND_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class SwapTableCommand : public QUndoCommand
{
public:
    SwapTableCommand(const int &targetindex
                     , const int &tablebefore
                     , const int &tableafter
                     , const QString objname
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
//    QString m_newstr;
//    QString m_oldstr;
    QList<QList<QStringList> *> *m_cache;

    int SKIP;

};

#endif // SWAPTABLECOMMAND_H
