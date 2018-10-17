#ifndef EDITLOOPINFCHECK_H
#define EDITLOOPINFCHECK_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckEditLoopInf : public QUndoCommand
{
public:
    CheckEditLoopInf(const int &targetindex
                     , const bool &newcheck
                     , QList<QList<QStringList> *> *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

private:
   int m_targetindex;
   QString m_oldcheck;
   QString m_newcheck;
   QList<QList<QStringList> *> *m_cache;
   //init generator
   ProcessXmlListGenerator pxlg;

};

#endif // EDITLOOPINFCHECK_H
