#ifndef EDITSEARCHINPUTCHECK_H
#define EDITSEARCHINPUTCHECK_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckEditSearchInput : public QUndoCommand
{
public:
    CheckEditSearchInput(const int &targetindex
                         , const bool &newcheck
                         , QList<QList<QStringList> *> *cache
                         , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
   int getxmlpos();

   int m_targetindex;
   QString m_oldcheck;
   QString m_newcheck;
   QList<QList<QStringList> *> *m_cache;
   QHash<int, int> xmlpos;

   const static int SUBPOS = 1;
};

#endif // EDITSEARCHINPUTCHECK_H