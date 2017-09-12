#ifndef EXTRAFUNCTIONSCOMBOBOX_H
#define EXTRAFUNCTIONSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>

class ExtraFunctionsComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ExtraFunctionsComboBox(QObject *parent = 0);
    ~ExtraFunctionsComboBox();

    QString getCurrentVariant();
    void reloadComboBoxItem();
public slots:
    void editAction(QString variant);
private:
    ExtrafunctionsXmlBuilder *builder;
};

#endif // EXTRAFUNCTIONSCOMBOBOX_H
