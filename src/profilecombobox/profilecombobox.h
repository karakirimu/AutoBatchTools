#ifndef PROFILECOMBOBOX_H
#define PROFILECOMBOBOX_H

#include "profilecombobox_global.h"

#include <QComboBox>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <../profilexmlbuilder/profilexmlbuilder.h>
#include <../processxmlbuilder/processxmlbuilder.h>

class PROFILECOMBOBOXSHARED_EXPORT ProfileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ProfileComboBox(QObject *parent = 0);
    ~ProfileComboBox();

    QString getCurrentFileName();
    QString getCurrentFileName(int index);
    int getIndexFromFileName(QString filepath);
public slots:
    void reloadComboBoxItem();
    void addItemAction();
    void deleteItemAction();

private:
    ProfileXmlBuilder *builder;
    ProcessXmlBuilder *pbuilder;
};

#endif // PROFILECOMBOBOX_H
