#ifndef VARIANTCOMBOBOX_H
#define VARIANTCOMBOBOX_H


#include <QComboBox>
#include <QWidget>
#include <../processxmlbuilder/processxmlbuilder.h>
#include <../stringxmlbuilder/stringxmlbuilder.h>
#include <../variantconverter/variantconverter.h>

class VariantComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit VariantComboBox(QObject *parent = 0);
    ~VariantComboBox();
    void reloadComboBoxItem();

public slots:
    void setProfileFileName(QString filename);

private:
    StringXmlBuilder *sbuilder;
    ProcessXmlBuilder *pbuilder;
    static const int PEMAGIC = 1;
};

#endif // VARIANTCOMBOBOX_H
