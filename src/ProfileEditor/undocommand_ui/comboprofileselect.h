#ifndef COMBOPROFILESELECT_H
#define COMBOPROFILESELECT_H

#include <QUndoCommand>
#include <../processxmllistgenerator/processxmllistgenerator.h>

class ComboProfileSelect : public QUndoCommand
{
public:
    ComboProfileSelect(const int &targetindex
                       , const QString newstring
                       , const QString newfile
                       , QList<QList<QStringList> *> *cache
                       , QUndoCommand *parent = nullptr);

   void undo() override;
   void redo() override;

   int id() const override;
   bool mergeWith(const QUndoCommand *other) override;

   QString m_newstring;
   QString m_newfile;
private:
   int m_targetindex;
   QString m_oldstring;
   QString m_oldfile;
   QList<QList<QStringList> *> *m_cache;
   QHash<int, int> posinfo;
};

#endif // COMBOPROFILESELECT_H
