#ifndef STRINGPROJECTNAME_H
#define STRINGPROJECTNAME_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class StringProjectName : public QUndoCommand
{
public:
    StringProjectName(const int &targetindex
                      , QString newstring
                      , QList<QList<QStringList> *> *cache
                      , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;

    QString m_newstring;
private:
    int m_targetindex;
    QString m_oldstring;
    QList<QList<QStringList> *> *m_cache;
    QHash<int, int> posinfo;
};

#endif // STRINGPROJECTNAME_H
