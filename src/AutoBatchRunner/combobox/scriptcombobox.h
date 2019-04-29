#ifndef EXTRAFUNCTIONSCOMBOBOX_H
#define EXTRAFUNCTIONSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <../pluginsxmlbuilder/pluginsxmlbuilder.h>

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
    PluginsXmlBuilder *builder;
};

#endif // EXTRAFUNCTIONSCOMBOBOX_H
