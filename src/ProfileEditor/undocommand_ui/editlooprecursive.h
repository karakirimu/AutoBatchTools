#ifndef EDITLOOPRECURSIVE_H
#define EDITLOOPRECURSIVE_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class EditLoopRecursive : public QUndoCommand
{
public:
    EditLoopRecursive(const int &targetindex
                      , int newvalue
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
     QList<QList<QStringList> *> *m_cache;
//     QHash<int, int> posinfo;
     ProcessXmlListGenerator pxlg;

};

#endif // EDITLOOPRECURSIVE_H
