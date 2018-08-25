#ifndef EDITTIMEOUTCHECKBOX_H
#define EDITTIMEOUTCHECKBOX_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckEditTimeout : public QUndoCommand
{
public:
    CheckEditTimeout(const int &targetindex
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
};

#endif // EDITTIMEOUTCHECKBOX_H
