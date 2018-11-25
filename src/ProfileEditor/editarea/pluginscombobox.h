#ifndef EXTRAFUNCTIONSCOMBOBOX_H
#define EXTRAFUNCTIONSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QPluginLoader>
#include <QMessageBox>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>
#include <../plugins/ExtraPluginInterface/extraplugininterface.h>
#include <QFileDialog>

class ExtraFunctionsComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void pluginChanged(bool);

public:
    explicit ExtraFunctionsComboBox(QObject *parent = nullptr);
    ~ExtraFunctionsComboBox();

    QString getCurrentExtraFile();
    void reloadComboBoxItem();
public slots:
    void addItemAction();
//    void editAction(QString variant);
    void deleteAction();
    void pluginCheckAction(int index);
private:
    ExtrafunctionsXmlBuilder *builder;
};

#endif // EXTRAFUNCTIONSCOMBOBOX_H
