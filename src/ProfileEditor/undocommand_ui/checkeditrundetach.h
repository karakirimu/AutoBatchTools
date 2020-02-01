#ifndef CHECKEDITSKIPRUN_H
#define CHECKEDITSKIPRUN_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckEditRunDetach : public QUndoCommand
{
public:
    CheckEditRunDetach(const int &targetindex
                     , const bool &newcheck
                     , QList<QList<QStringList> *> *cache
                     , QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;

    int id() const override;

 private:
    int m_targetindex;
    QString m_oldcheck;
    QString m_newcheck;
    QList<QList<QStringList> *> *m_cache;
    ProcessXmlListGenerator pxlg;

 };

#endif // CHECKEDITSKIPRUN_H
