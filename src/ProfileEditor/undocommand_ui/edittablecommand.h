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

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

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

    ProcessXmlListGenerator pxlg;

    int SKIP;

};

#endif // EDITTABLECOMMAND_H
