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
    PROFILECOMBOBOXSHARED_EXPORT explicit ProfileComboBox(QObject *parent = 0);
    PROFILECOMBOBOXSHARED_EXPORT ~ProfileComboBox();

    PROFILECOMBOBOXSHARED_EXPORT QString getCurrentFileName();
    PROFILECOMBOBOXSHARED_EXPORT QString getCurrentFileName(int index);
public slots:
    void reloadComboBoxItem();
    void addItemAction();
    void deleteItemAction();

private:
    ProfileXmlBuilder *builder;
    ProcessXmlBuilder *pbuilder;
};

#endif // PROFILECOMBOBOX_H
