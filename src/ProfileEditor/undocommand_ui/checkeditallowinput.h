#ifndef EDITALLOWINPUT_H
#define EDITALLOWINPUT_H

#include <QHash>
#include <QUndoCommand>
#include <QRegularExpression>
#include <../processxmllistgenerator/processxmllistgenerator.h>
#include <../variantconverter/variantconverter.h>

class CheckEditAllowInput : public QUndoCommand
{
public:
    CheckEditAllowInput(const int &targetindex
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

#endif // EDITALLOWINPUT_H
