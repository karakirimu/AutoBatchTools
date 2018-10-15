#ifndef STRINGFILEOUTPUT_H
#define STRINGFILEOUTPUT_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class StringFileOutput : public QUndoCommand
{
public:
    StringFileOutput(const int &targetindex
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
//    QHash<int, int> posinfo;
    ProcessXmlListGenerator pxlg;

};

#endif // STRINGFILEOUTPUT_H
