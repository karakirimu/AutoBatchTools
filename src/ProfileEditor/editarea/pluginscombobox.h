#ifndef PLUGINSCOMBOBOX_H
#define PLUGINSCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QPluginLoader>
#include <QMessageBox>
#include <../extrafunctionsxmlbuilder/extrafunctionsxmlbuilder.h>
#include <../plugins/ExtraPluginInterface/extraplugininterface.h>
#include <QFileDialog>

class PluginsComboBox : public QComboBox
{
    Q_OBJECT

signals:
    void pluginChanged(bool);

public:
    explicit PluginsComboBox(QObject *parent = nullptr);
    ~PluginsComboBox();

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

#endif // PLUGINSCOMBOBOX_H
